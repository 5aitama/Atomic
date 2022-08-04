#include "context.h"
#include "context_private.h"
#include <wgpu.h>

AtomicContext context = NULL;

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

void atm_init(Surface surface) {
    
    wgpuSetLogCallback(wgpu_log_callback);
    wgpuSetLogLevel(WGPULogLevel_Warn);

    WGPUInstance wgpu_instance  = NULL;
    WGPUAdapter wgpu_adapter    = NULL;
    WGPUDevice wgpu_device      = NULL;
    WGPUSurface wgpu_surface    = (WGPUSurface)surface_get_raw(&surface);

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

    context = (AtomicContext)malloc(sizeof(AtomicContext_s));
    *context = (AtomicContext_s){
        .instance           = wgpu_instance,
        .adapter            = wgpu_adapter,
        .device             = wgpu_device,
        .swapchain_format   = wgpu_swapchain_format,
        .swapchain          = wgpu_swapchain,
        .surface            = surface,
        .swapchain_width    = width,
        .swapchain_height   = height,
        .render_texture     = NULL,
        .queue              = wgpuDeviceGetQueue(wgpu_device),
        .unsubmited_command_buffers = NULL,
        .unsubmited_command_buffers_count = 0, 
    };
}

void atm_fini() {
    if (context != NULL) free(context);
}

void atm_begin_render() {
    WGPUTextureView next_texture = NULL;

    for (int attempt = 0; attempt < 2; attempt++) {

        uint32_t width  = 0;
        uint32_t height = 0;

        surface_get_size(&context->surface, &width, &height);

        if (width != context->swapchain_width || height != context->swapchain_height) {
            context->swapchain_width  = width;
            context->swapchain_height = height;

            context->swapchain = wgpuDeviceCreateSwapChain(
                context->device, 
                (WGPUSurface)surface_get_raw(&context->surface),
                &(WGPUSwapChainDescriptor){
                    .usage       = WGPUTextureUsage_RenderAttachment,
                    .format      = context->swapchain_format,
                    .width       = context->swapchain_width,
                    .height      = context->swapchain_height,
                    .presentMode = WGPUPresentMode_Fifo,
                }
            );
        }

        next_texture = wgpuSwapChainGetCurrentTextureView(context->swapchain);

        if (attempt == 0 && !next_texture) {
            printf("wgpuSwapChainGetCurrentTextureView() failed; trying to create a new swap chain...\n");
            context->swapchain_width  = 0;
            context->swapchain_height = 0;
            continue;
        }

        break;
    }

    if (!next_texture) {
        printf("Cannot acquire next swap chain texture\n");
    }

    context->render_texture = next_texture;
}

void atm_end_render() {
    wgpuTextureViewDrop(context->render_texture);
    wgpuQueueSubmit(context->queue, context->unsubmited_command_buffers_count, context->unsubmited_command_buffers);
    
    free(context->unsubmited_command_buffers);
    context->unsubmited_command_buffers = NULL;
    context->unsubmited_command_buffers_count = 0;
    
    wgpuSwapChainPresent(context->swapchain);
}