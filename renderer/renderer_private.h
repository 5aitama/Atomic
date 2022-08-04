#ifndef SAITAMA_ATOMIC_CORE_RENDERER_PRIVATE_H
#define SAITAMA_ATOMIC_CORE_RENDERER_PRIVATE_H

#include <stdlib.h>
#include <wgpu.h>

typedef struct Renderer_t {
    WGPUInstance        instance;
    WGPUAdapter         adapter;
    WGPUDevice          device;
    WGPUTextureFormat   swapchain_format;
    WGPUSwapChain       swapchain;
    Surface*            surface;
    uint32_t            swapchain_width;
    uint32_t            swapchain_height;
} Renderer_t;

#endif