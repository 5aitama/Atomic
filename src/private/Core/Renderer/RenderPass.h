#ifndef ATOMIC_CORE_RENDERER_RENDER_PASS_PRIVATE_H
#define ATOMIC_CORE_RENDERER_RENDER_PASS_PRIVATE_H

#include <wgpu.h>

typedef struct RenderPass_s {
    WGPUCommandEncoder              encoder;
    WGPURenderPassEncoder           pass_encoder;
    WGPURenderPassColorAttachment*  color_attachments;
    uint32_t                        color_attachments_count;
} RenderPass_s;

#endif