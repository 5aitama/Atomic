#ifndef SAITAMA_ATOMIC_CONTEXT_PRIVATE_H
#define SAITAMA_ATOMIC_CONTEXT_PRIVATE_H

#include <wgpu.h>

typedef struct AtomicContext_s {
    WGPUInstance instance;
    WGPUAdapter adapter;
    WGPUDevice device;
    WGPUTextureFormat swapchain_format;
    WGPUSwapChain swapchain;
    Surface surface;
    uint32_t swapchain_width;
    uint32_t swapchain_height;
    WGPUTextureView render_texture;
    WGPUCommandBuffer* unsubmited_command_buffers;
    uint32_t unsubmited_command_buffers_count;
    WGPUQueue queue;
} AtomicContext_s;

#endif