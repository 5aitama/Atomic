#include <Atomic/Core/Renderer/PassEncoder.h>
#include <Atomic/Core/Renderer/RenderPass.h>

#include "PassEncoder.h"
#include "RenderPass.h"
#include "Context.h"

void pass_encoder_init(PassEncoder* pass_encoder) {
    WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(ctx->device, &(WGPUCommandEncoderDescriptor) {
        .label          = NULL,
        .nextInChain    = NULL,
    });

    *pass_encoder = (PassEncoder)malloc(sizeof(PassEncoder_s));
    *(*pass_encoder) = (PassEncoder_s) {
        .encoder                = encoder,
        .buffer                 = NULL,
        .render_passes          = NULL,
        .render_passes_count    = 0,
        .compute_passes         = NULL,
        .compute_passes_count   = 0,
    };
}

RenderPass create_render_pass(PassEncoder encoder) {
    RenderPass pass = (RenderPass)malloc(sizeof(RenderPass_s));
    *pass = (RenderPass_s) {
        .encoder                    = encoder->encoder,
        .pass_encoder               = NULL,
        .color_attachments          = NULL,
        .color_attachments_count    = 0,
    };

    return pass;
}

void delete_render_pass(RenderPass pass) {
    if (pass->color_attachments_count > 0)
        free(pass->color_attachments);
    
    free(pass);
}

ComputePass create_compute_pass(PassEncoder encoder) {
    // TODO: Implement this...
    return NULL;
}

void delete_compute_pass(ComputePass pass) {
    // TODO: Implement this...
}

void pass_encoder_fini(PassEncoder* pass_encoder) {
    if (pass_encoder == NULL || *pass_encoder == NULL) return;
    
    for (uint32_t i = 0; i < (*pass_encoder)->render_passes_count; i++)
        delete_render_pass((*pass_encoder)->render_passes[i]);

    if ((*pass_encoder)->render_passes_count > 0)
        free((*pass_encoder)->render_passes);

    for (uint32_t i = 0; i < (*pass_encoder)->compute_passes_count; i++)
        delete_compute_pass((*pass_encoder)->compute_passes[i]);

    if ((*pass_encoder)->compute_passes_count > 0)
        free((*pass_encoder)->compute_passes);

    if ((*pass_encoder)->buffer != NULL)
        wgpuCommandBufferDrop((*pass_encoder)->buffer);

    // wgpuCommandEncoderDrop((*pass_encoder)->encoder);

    free(*pass_encoder);
}

void pass_encoder_encode_passes(PassEncoder encoder, PassDescriptor* passes, const uint32_t count, void* user_data) {
    uint32_t render_passes_count = 0;
    uint32_t compute_passes_count = 0;

    for (uint32_t i = 0; i < count; i++) {
        if (passes[i].type == PassType_RenderPass) {
            render_passes_count++;
        } else if(passes[i].type == PassType_ComputePass) {
            compute_passes_count++;
        } else {
            continue;
        }
    }

    encoder->render_passes       = (RenderPass*)malloc(sizeof(RenderPass_s) * render_passes_count);
    encoder->render_passes_count = render_passes_count;

    encoder->compute_passes       = (ComputePass*)malloc(sizeof(ComputePass_s) * compute_passes_count);
    encoder->compute_passes_count = compute_passes_count;

    for (uint32_t i = 0; i < count; i++) {
        if (passes[i].type == PassType_RenderPass) {
            RenderPass render_pass = create_render_pass(encoder);
            passes[i].func(render_pass, user_data);
            encoder->render_passes[i] = render_pass;
        } else if (passes[i].type == PassType_ComputePass) {
            ComputePass compute_pass = create_compute_pass(encoder);
            passes[i].func(compute_pass, user_data);
            encoder->compute_passes[i] = compute_pass;
        }
    }
}

void pass_encoder_submit(PassEncoder* encoders, const uint32_t count) {
    WGPUCommandBuffer* buffers = (WGPUCommandBuffer*)malloc(sizeof(WGPUCommandBuffer) * count);

    for (uint32_t i = 0; i < count; i++) {
        buffers[i] = wgpuCommandEncoderFinish(encoders[i]->encoder, &(WGPUCommandBufferDescriptor) {
            .label       = NULL,
            .nextInChain = NULL,
        });
    }

    wgpuQueueSubmit(ctx->queue, count, buffers);
    free(buffers);
}
