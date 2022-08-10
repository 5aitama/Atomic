#include <Atomic/Core/Renderer/Context.h>
#include "Surface.h"
#include "Context.h"

#include <stdio.h>
#include <stdlib.h>

RendererContext_s* ctx = NULL;

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

static void wgpu_handle_device_lost(WGPUDeviceLostReason reason, char const * message, void * userdata) {
    printf("DEVICE LOST (%d): %s\n", reason, message);
}

static void wgpu_handle_uncaptured_error(WGPUErrorType type, char const * message, void * userdata) {
    printf("UNCAPTURED ERROR (%d): %s\n", type, message);
}

void wgpu_request_adapter_callback(WGPURequestAdapterStatus status, WGPUAdapter received, const char *message, void *userdata) {
  *(WGPUAdapter *)userdata = received;
}

void wgpu_request_device_callback(WGPURequestDeviceStatus status, WGPUDevice received, const char *message, void *userdata) {
  *(WGPUDevice *)userdata = received;
}

void renderer_init(Surface surface) {
    wgpuSetLogCallback(wgpu_log_callback);
    wgpuSetLogLevel(WGPULogLevel_Warn);

    WGPUInstance wgpu_instance  = NULL;
    WGPUAdapter  wgpu_adapter   = NULL;
    WGPUDevice   wgpu_device    = NULL;
    WGPUSurface  wgpu_surface   = surface->surface;

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
    surface_get_size(&surface, &width, &height);

    wgpuDeviceSetUncapturedErrorCallback(wgpu_device, wgpu_handle_uncaptured_error, NULL);
    wgpuDeviceSetDeviceLostCallback(wgpu_device, wgpu_handle_device_lost, NULL);

    WGPUSwapChain wgpu_swapchain = wgpuDeviceCreateSwapChain(wgpu_device, wgpu_surface, &(WGPUSwapChainDescriptor){
        .usage       = WGPUTextureUsage_RenderAttachment,
        .format      = wgpu_swapchain_format,
        .width       = width,
        .height      = height,
        .presentMode = WGPUPresentMode_Fifo,
    });

    ctx = (RendererContext_s*)malloc(sizeof(RendererContext_s));
    *ctx = (RendererContext_s){
        .instance           = wgpu_instance,
        .adapter            = wgpu_adapter,
        .device             = wgpu_device,
        .swapchain = (RendererSwapchain_s) {
            .format = wgpu_swapchain_format,
            .width  = width,
            .height = height,
            .swapchain = wgpu_swapchain,
        },
        .surface            = surface,
        .render_texture     = NULL,
        .queue              = wgpuDeviceGetQueue(wgpu_device),
    };
}

void renderer_fini() {
    if (ctx != NULL) {
        
        if (ctx->render_texture != NULL)
            wgpuTextureViewDrop(ctx->render_texture);
        
        wgpuDeviceDrop(ctx->device);
        wgpuAdapterDrop(ctx->adapter);
    }
}

void renderer_aquire_texture() {
    WGPUTextureView next_texture = NULL;

    for (int attempt = 0; attempt < 2; attempt++) {

        uint32_t width  = 0;
        uint32_t height = 0;

        surface_get_size(&ctx->surface, &width, &height);

        if (width != ctx->swapchain.width || height != ctx->swapchain.height) {
            ctx->swapchain.width  = width;
            ctx->swapchain.height = height;

            ctx->swapchain.swapchain = wgpuDeviceCreateSwapChain(ctx->device, ctx->surface->surface,
                &(WGPUSwapChainDescriptor){
                    .usage       = WGPUTextureUsage_RenderAttachment,
                    .format      = ctx->swapchain.format,
                    .width       = ctx->swapchain.width,
                    .height      = ctx->swapchain.height,
                    .presentMode = WGPUPresentMode_Fifo,
                }
            );
        }

        next_texture = wgpuSwapChainGetCurrentTextureView(ctx->swapchain.swapchain);

        if (attempt == 0 && !next_texture) {
            printf("wgpuSwapChainGetCurrentTextureView() failed; trying to create a new swap chain...\n");
            ctx->swapchain.width  = 0;
            ctx->swapchain.height = 0;
            continue;
        }

        break;
    }

    if (!next_texture) {
        printf("Cannot acquire next swap chain texture\n");
    }

    ctx->render_texture = next_texture;
}

void renderer_present() {
    wgpuTextureViewDrop(ctx->render_texture);
    wgpuSwapChainPresent(ctx->swapchain.swapchain);
}