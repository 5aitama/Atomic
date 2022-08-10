#include <Atomic/command_encoder.h>
#include "Atomic/context.h"
#include "../context/context_private.h"
#include "command_encoder.h"

extern AtomicContext context;

void command_encoder_init(CommandEncoder* command_encoder) {
    WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(context->device, &(WGPUCommandEncoderDescriptor) {
        .label          = NULL,
        .nextInChain    = NULL,
    });

    *command_encoder = (CommandEncoder)malloc(sizeof(CommandEncoder_s));
    *(*command_encoder) = (CommandEncoder_s) {
        .encoder = encoder,
    };
}

void command_encoder_fini(CommandEncoder* command_encoder) {
    if (command_encoder == NULL || *command_encoder == NULL) return;
    wgpuCommandEncoderDrop((*command_encoder)->encoder);
    free(*command_encoder);
}

void command_encoder_start_record_render_pass(CommandEncoder command_encoder) {
    wgpuCommandEncoderBeginRenderPass(command_encoder->encoder, &(WGPURenderPassDescriptor) {
        .colorAttachmentCount = 1,
        .colorAttachments = &(WGPURenderPassColorAttachment) {
            .clearValue = (WGPUColor) {
                .r = 0.01,
                .g = 0.01,
                .b = 0.01,
                .a = 1.00,
            },
            .loadOp         = WGPULoadOp_Clear,
            .storeOp        = WGPUStoreOp_Store,
            .resolveTarget  = NULL,
            .view           = context->render_texture,
        },
        .depthStencilAttachment = NULL,
        .label                  = NULL,
        .nextInChain            = NULL,
        .occlusionQuerySet      = NULL,
        .timestampWriteCount    = 0,
        .timestampWrites        = NULL,
    });
}