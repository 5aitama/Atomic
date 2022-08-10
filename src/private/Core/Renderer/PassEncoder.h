#ifndef ATOMIC_CORE_RENDERER_PASS_ENCODER_PRIVATE_H
#define ATOMIC_CORE_RENDERER_PASS_ENCODER_PRIVATE_H

#include <wgpu.h>
#include <Atomic/Core/Renderer/RenderPass.h>
#include <stdint.h>

typedef struct ComputePass_s { void* n; } ComputePass_s;
typedef ComputePass_s* ComputePass;

typedef struct PassEncoder_s {
    WGPUCommandEncoder  encoder;
    WGPUCommandBuffer   buffer;

    RenderPass*         render_passes;
    uint32_t            render_passes_count;

    ComputePass*        compute_passes;
    uint32_t            compute_passes_count;
} PassEncoder_s;

#endif