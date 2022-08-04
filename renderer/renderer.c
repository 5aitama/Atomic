#include "renderer.h"
#include "renderer_private.h"
#include <stdio.h>

void wgpu_log_callback(WGPULogLevel level, const char* msg) {
    const char* level_str;
    switch (level) {
        case WGPULogLevel_Error: level_str = "Error"; break;
        case WGPULogLevel_Warn:  level_str = "Warn";  break;
        case WGPULogLevel_Info:  level_str = "Info";  break;
        case WGPULogLevel_Debug: level_str = "Debug"; break;
        case WGPULogLevel_Trace: level_str = "Trace"; break;
        default: level_str = "Unknown Level";
    }

    printf("[%s] %s\n", level_str, msg);
}

static void wgpu_handle_device_lost(WGPUDeviceLostReason reason, char const * message, void * userdata)
{
    printf("DEVICE LOST (%d): %s\n", reason, message);
}

static void wgpu_handle_uncaptured_error(WGPUErrorType type, char const * message, void * userdata)
{
    printf("UNCAPTURED ERROR (%d): %s\n", type, message);
}

void wgpu_request_adapter_callback(WGPURequestAdapterStatus status,
                              WGPUAdapter received, const char *message,
                              void *userdata) {

  *(WGPUAdapter *)userdata = received;
}

void wgpu_request_device_callback(WGPURequestDeviceStatus status,
                             WGPUDevice received, const char *message,
                             void *userdata) {

  *(WGPUDevice *)userdata = received;
}

int init_renderer(Renderer* renderer, Surface* surface) {
    wgpuSetLogCallback(wgpu_log_callback);
    wgpuSetLogLevel(WGPULogLevel_Warn);

    WGPUInstance wgpu_instance  = NULL;
    WGPUAdapter wgpu_adapter    = NULL;
    WGPUDevice wgpu_device      = NULL;
    WGPUSurface wgpu_surface    = (WGPUSurface)surface_get_raw(surface);

    wgpuInstanceRequestAdapter(wgpu_instance, &(WGPURequestAdapterOptions){
        .compatibleSurface      = wgpu_surface,
        .forceFallbackAdapter   = false,
        .powerPreference        = WGPUPowerPreference_HighPerformance,
        .nextInChain            = NULL,
    }, wgpu_request_adapter_callback, &wgpu_adapter);

    wgpuAdapterRequestDevice(wgpu_adapter, &(WGPUDeviceDescriptor){
        .label = NULL,
        .defaultQueue = (WGPUQueueDescriptor){ .label = NULL, .nextInChain = NULL },
        .nextInChain = (const WGPUChainedStruct*)&(WGPUDeviceExtras){
            .chain = (WGPUChainedStruct){
                .next = NULL,
                .sType = (WGPUSType)WGPUSType_DeviceExtras,
            },
            .label = "Device",
            .tracePath = NULL,
        },
        .requiredLimits = &(WGPURequiredLimits){
            .nextInChain = NULL,
            .limits = (WGPULimits){
                .maxBindGroups = 1,
            },
        },
    }, wgpu_request_device_callback, &wgpu_device);

    WGPUTextureFormat wgpu_swapchain_format = wgpuSurfaceGetPreferredFormat(wgpu_surface, wgpu_adapter);

    uint32_t width, height;
    surface_get_size(surface, &width, &height);

    wgpuDeviceSetUncapturedErrorCallback(wgpu_device, wgpu_handle_uncaptured_error, NULL);
    wgpuDeviceSetDeviceLostCallback(wgpu_device, wgpu_handle_device_lost, NULL);

    WGPUSwapChain wgpu_swapchain = wgpuDeviceCreateSwapChain(wgpu_device, wgpu_surface, &(WGPUSwapChainDescriptor){
        .usage       = WGPUTextureUsage_RenderAttachment,
        .format      = wgpu_swapchain_format,
        .width       = width,
        .height      = height,
        .presentMode = WGPUPresentMode_Fifo,
    });

    *renderer = (Renderer)malloc(sizeof(Renderer_t));

    if (*renderer == NULL) {
		printf("Failed to allocate memory for create a new renderer !\n");
		return EXIT_FAILURE;
	} else {
		*(*renderer) = (Renderer_t){
			.instance           = wgpu_instance,
            .adapter            = wgpu_adapter,
            .device             = wgpu_device,
            .swapchain_format   = wgpu_swapchain_format,
            .swapchain          = wgpu_swapchain,
            .surface            = surface,
            .swapchain_width    = width,
            .swapchain_height   = height,
		};
	}

    return EXIT_SUCCESS;
}

int fini_renderer(Renderer* renderer) {
    if(renderer == NULL || *renderer == NULL) return EXIT_FAILURE;
    
    // wgpuDeviceDestroy((*renderer)->device);

    free(*renderer);
    return EXIT_SUCCESS;
}

int render_frame(Renderer* renderer) {
    WGPUTextureView next_texture = NULL;

    for (int attempt = 0; attempt < 2; attempt++) {

        uint32_t width  = 0;
        uint32_t height = 0;

        surface_get_size((*renderer)->surface, &width, &height);

        if (width != (*renderer)->swapchain_width || height != (*renderer)->swapchain_height) {
            (*renderer)->swapchain_width  = width;
            (*renderer)->swapchain_height = height;

            (*renderer)->swapchain = wgpuDeviceCreateSwapChain(
                (*renderer)->device, 
                (WGPUSurface)surface_get_raw((*renderer)->surface),
                &(WGPUSwapChainDescriptor){
                    .usage       = WGPUTextureUsage_RenderAttachment,
                    .format      = (*renderer)->swapchain_format,
                    .width       = (*renderer)->swapchain_width,
                    .height      = (*renderer)->swapchain_height,
                    .presentMode = WGPUPresentMode_Fifo,
                }
            );
        }

        next_texture = wgpuSwapChainGetCurrentTextureView((*renderer)->swapchain);

        if (attempt == 0 && !next_texture) {
            printf("wgpuSwapChainGetCurrentTextureView() failed; trying to create a new swap chain...\n");
            (*renderer)->swapchain_width  = 0;
            (*renderer)->swapchain_height = 0;
            continue;
        }

        break;
    }

    if (!next_texture) {
        printf("Cannot acquire next swap chain texture\n");
        return EXIT_FAILURE;
    }

    WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder((*renderer)->device, &(WGPUCommandEncoderDescriptor){
        .label = "Command Encoder"
    });

    WGPURenderPassEncoder renderPass = wgpuCommandEncoderBeginRenderPass(encoder, &(WGPURenderPassDescriptor){
        .colorAttachments = &(WGPURenderPassColorAttachment){
                .view           = next_texture,
                .resolveTarget  = 0,
                .loadOp         = WGPULoadOp_Clear,
                .storeOp        = WGPUStoreOp_Store,
                .clearValue     = (WGPUColor){
                    .r = 0.01,
                    .g = 0.01,
                    .b = 0.01,
                    .a = 1.0,
                },
            },
        .colorAttachmentCount = 1,
        .depthStencilAttachment = NULL,
    });

    // wgpuRenderPassEncoderSetPipeline(renderPass, pipeline);
    // wgpuRenderPassEncoderDraw(renderPass, 3, 1, 0, 0);
    wgpuRenderPassEncoderEnd(renderPass);
    wgpuTextureViewDrop(next_texture);

    WGPUQueue queue = wgpuDeviceGetQueue((*renderer)->device);
    WGPUCommandBuffer cmdBuffer = wgpuCommandEncoderFinish(encoder, &(WGPUCommandBufferDescriptor){
        .label = NULL
    });
    wgpuQueueSubmit(queue, 1, &cmdBuffer);
    wgpuSwapChainPresent((*renderer)->swapchain);

    return EXIT_SUCCESS;
}