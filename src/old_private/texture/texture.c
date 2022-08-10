#include "Atomic/texture.h"
#include "texture_private.h"

#include "Atomic/context.h"
#include "../context/context_private.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

extern AtomicContext context;

void texture2D_init(Texture2D* texture2d, const char* path) {
    uint32_t width, height, bpp;
    uint8_t* rgb_image = stbi_load(path, (int*)&width, (int*)&height, (int*)&bpp, 4);
    bpp = 4;
    
    printf("The image size: %u x %u (%ubpp)\n", width, height, bpp);
    printf("The image data size: %u\n", width * height * bpp);

    WGPUTexture texture = wgpuDeviceCreateTexture(context->device, &(WGPUTextureDescriptor) {
        .label          = NULL,
        .dimension      = WGPUTextureDimension_2D,
        .format         = WGPUTextureFormat_RGBA8UnormSrgb,
        .mipLevelCount  = 1,
        .nextInChain    = NULL,
        .sampleCount    = 1,
        .size = (WGPUExtent3D) {
            .depthOrArrayLayers = 1,
            .width              = width,
            .height             = height,
        },
        .usage              = WGPUTextureUsage_TextureBinding | WGPUTextureUsage_CopyDst,
        .viewFormatCount    = 0,
        .viewFormats        = NULL,
    });

    WGPUTextureView texture_view = wgpuTextureCreateView(texture, &(WGPUTextureViewDescriptor) {
        .label           = NULL,
        .nextInChain     = NULL,
        .aspect          = WGPUTextureAspect_All,
        .arrayLayerCount = 1,
        .mipLevelCount   = 1,
        .baseArrayLayer  = 0,
        .baseMipLevel    = 0,
        .dimension       = WGPUTextureViewDimension_2D,
        .format          = WGPUTextureFormat_RGBA8UnormSrgb,
    });

    wgpuQueueWriteTexture(context->queue, &(WGPUImageCopyTexture){
        .texture = texture,
        .mipLevel = 0,
        .origin = (WGPUOrigin3D) {
            .x = 0,
            .y = 0,
            .z = 0,
        },
        .aspect = WGPUTextureAspect_All,
    }, rgb_image, sizeof(uint8_t) * width * height * bpp, &(WGPUTextureDataLayout) {
        .bytesPerRow    = bpp * width,
        .offset         = 0,
        .rowsPerImage   = height,
        .nextInChain    = NULL,
    }, &(WGPUExtent3D){
        .depthOrArrayLayers = 1,
        .width  = width,
        .height = height,
    });

    stbi_image_free(rgb_image);

    *texture2d = (Texture2D)malloc(sizeof(Texture2D_t));
    *(*texture2d) = (Texture2D_t) {
        .texture        = texture,
        .texture_view   = texture_view,
    };
}

void texture2D_fini(Texture2D* texture2d) {
    wgpuTextureViewDrop((*texture2d)->texture_view);
    wgpuTextureDrop((*texture2d)->texture);
    free(*texture2d);
}