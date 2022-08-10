#include "RenderPass.h"
#include <Atomic/Core/Renderer/RenderPass.h>
#include "Context.h"
#include <stdio.h>

void render_pass_begin(RenderPass pass) {

    if (pass->color_attachments_count > 0) {
        WGPURenderPassDescriptor desc = (WGPURenderPassDescriptor) {
            .colorAttachments       = pass->color_attachments,
            .colorAttachmentCount   = pass->color_attachments_count,
        };

        WGPURenderPassEncoder pass_encoder = wgpuCommandEncoderBeginRenderPass(pass->encoder, &desc);

        free(pass->color_attachments);
        pass->color_attachments_count = 0;

        if (pass->pass_encoder == NULL)
            pass->pass_encoder = pass_encoder;
    }
}

void render_pass_end(RenderPass pass) {
    if (pass->pass_encoder != NULL)
        wgpuRenderPassEncoderEnd(pass->pass_encoder);
}

void render_pass_add_color_attachment(RenderPass pass, const float r, const float g, const float b, const float a) {
    if (pass->color_attachments_count == 0) {
        pass->color_attachments = (WGPURenderPassColorAttachment*)malloc(sizeof(WGPURenderPassColorAttachment));
    } else {
        pass->color_attachments = (WGPURenderPassColorAttachment*)realloc(
            pass->color_attachments, 
            sizeof(WGPURenderPassColorAttachment) * pass->color_attachments_count + 1
        );
    }

    pass->color_attachments_count++;
    pass->color_attachments[pass->color_attachments_count - 1] = (WGPURenderPassColorAttachment){
        .view           = ctx->render_texture,
        .resolveTarget  = NULL,
        .loadOp         = WGPULoadOp_Clear,
        .storeOp        = WGPUStoreOp_Store,
        .clearValue     = (WGPUColor){
            .r = r,
            .g = g,
            .b = b,
            .a = a,
        },
    };
}