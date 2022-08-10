#include "Atomic/render_pass.h"
#include "Atomic/context.h"
#include "../../context/context_private.h"
#include "../pipeline/render_pipeline_private.h"
#include "../../buffer/vertex/vertex_buffer_private.h"
#include "../../buffer/buffer_private.h"
#include "../../buffer/uniform/uniform_private.h"
#include "render_pass.h"

extern AtomicContext context;

typedef struct RenderPass_s {
    /**
     * A command encoder can record RenderPasses, ComputePasses, 
     * and transfer operations between driver-managed resources 
     * like Buffers and Textures.
     */
    WGPUCommandEncoder    render_pass_command_encoder;
    WGPURenderPassEncoder render_pass_encoder;
} RenderPass_s;

void render_pass_init(RenderPass* render_pass) {
    *render_pass = (RenderPass)malloc(sizeof(RenderPass_s));
    *(*render_pass) = (RenderPass_s) {
        .render_pass_command_encoder = NULL,
        .render_pass_encoder = NULL,
    };
}

void render_pass_begin(RenderPass render_pass) {
    WGPUCommandEncoder render_pass_command_encoder = wgpuDeviceCreateCommandEncoder(
        context->device, 
        &(WGPUCommandEncoderDescriptor){
            .label = "Command Encoder"
        }
    );

    WGPURenderPassEncoder render_pass_encoder = wgpuCommandEncoderBeginRenderPass(
        render_pass_command_encoder, 
        &(WGPURenderPassDescriptor){
            .colorAttachments = &(WGPURenderPassColorAttachment){
                    .view           = context->render_texture,
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
        }
    );

    render_pass->render_pass_encoder = render_pass_encoder;
    render_pass->render_pass_command_encoder = render_pass_command_encoder;
}

void render_pass_end(RenderPass render_pass) {
    wgpuRenderPassEncoderEnd(render_pass->render_pass_encoder);

    WGPUCommandBuffer command_buffer = wgpuCommandEncoderFinish(
        render_pass->render_pass_command_encoder,
        &(WGPUCommandBufferDescriptor){
            .label = NULL
        }
    );

    if (context->unsubmited_command_buffers == NULL) {
        context->unsubmited_command_buffers = (WGPUCommandBuffer*)malloc(sizeof(WGPUCommandBuffer));
    } else {
        context->unsubmited_command_buffers = (WGPUCommandBuffer*)realloc(
            context->unsubmited_command_buffers, 
            sizeof(WGPUCommandBuffer) * context->unsubmited_command_buffers_count + 1
        );
    }

    context->unsubmited_command_buffers[context->unsubmited_command_buffers_count] = command_buffer;
    context->unsubmited_command_buffers_count ++;
}

void render_pass_fini(RenderPass* render_pass) {
    free(*render_pass);
}                 

void render_pass_set_pipeline(RenderPass render_pass, RenderPipeline render_pipeline) {
    wgpuRenderPassEncoderSetPipeline(render_pass->render_pass_encoder, render_pipeline->render_pipeline);
}

void render_pass_draw(
    RenderPass render_pass, 
    const uint32_t vertex_count, 
    const uint32_t instance_count,
    const uint32_t first_vertex, 
    const uint32_t first_instance
) {
    wgpuRenderPassEncoderDraw(render_pass->render_pass_encoder, vertex_count, instance_count, first_vertex, first_instance);
}

void render_pass_set_vertex_buffer(RenderPass render_pass, VertexBuffer vertex_buffer) {
    wgpuRenderPassEncoderSetVertexBuffer(render_pass->render_pass_encoder, 0, vertex_buffer->buffer->buffer, 0, vertex_buffer->buffer->size);
}

void render_pass_set_uniform_groups(RenderPass render_pass, UniformGroup* uniform_groups, const uint32_t count) {
    for (uint32_t i = 0; i < count; i++)
        wgpuRenderPassEncoderSetBindGroup(render_pass->render_pass_encoder, i, uniform_groups[i]->bind_group, 0, NULL);
}

void render_pass_set_index_buffer(RenderPass render_pass, Buffer index_buffer, const IndexFormat format, const uint64_t offset, const uint64_t size) {
    wgpuRenderPassEncoderSetIndexBuffer(render_pass->render_pass_encoder, index_buffer->buffer, (WGPUIndexFormat)format, offset, size);
}

void render_pass_draw_indexed(
    RenderPass render_pass,
    const uint32_t index_count,
    const uint32_t instance_count,
    const uint32_t first_index,
    const uint32_t base_vertex,
    const uint32_t first_instance
) {
    wgpuRenderPassEncoderDrawIndexed(
        render_pass->render_pass_encoder, 
        index_count, 
        instance_count,
        first_index, 
        base_vertex, 
        first_instance
    );
}