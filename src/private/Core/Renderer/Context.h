#ifndef ATOMIC_CORE_RENDERER_CONTEXT_PRIVATE_H
#define ATOMIC_CORE_RENDERER_CONTEXT_PRIVATE_H

#include <wgpu.h>
#include <Atomic/Core/Renderer/Surface.h>

/**
 * @brief All stuff about the swapchain.
 */
typedef struct RendererSwapchain_s {
    /** The current swapchain width */
    uint32_t width;

    /** The current swapchain height */
    uint32_t height;

    /** The current swapchain format */
    WGPUTextureFormat format;

    /** The current swapchain */
    WGPUSwapChain swapchain;
} RendererSwapchain_s;

/**
 * @brief A Renderer context is similar to OpenGL context.
 */
typedef struct RendererContext_s {
    /** The current wgpu instance. */
    WGPUInstance instance;

    /** The current wgpu adapter. */
    WGPUAdapter adapter;

    /** The current wgpu device. */
    WGPUDevice device;

    /** The current swapchain. */
    RendererSwapchain_s swapchain;

    /** The current render texture. */
    WGPUTextureView render_texture;

    /** The current wgpu queue. */
    WGPUQueue queue;

    /** The current surface. */
    Surface surface;
} RendererContext_s;

/** 
 * The main rendering context. You must
 * check is it was null because it can
 * potentially be null if the user don't
 * initialize it.
 */
extern RendererContext_s* ctx;

#endif