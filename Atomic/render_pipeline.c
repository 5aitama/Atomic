#include "render_pipeline.h"
#include "render_pipeline_private.h"

#include "context.h"
#include "context_private.h"

#include "shader_private.h"
#include "vertex_buffer_private.h"

#include "buffer/uniform/uniform.h"
#include "buffer/uniform/uniform_private.h"

extern AtomicContext context;

void render_pipeline_init(RenderPipeline* render_pipeline, Shader shader, VertexBuffer vertex_buffer, const UniformGroup* uniform_groups, const uint32_t count) {
    
    WGPUBindGroupLayout* bind_group_layouts = (WGPUBindGroupLayout*)malloc(sizeof(WGPUBindGroupLayout) * count);

    for (uint32_t i = 0; i < count; i++)
        bind_group_layouts[i] = uniform_groups[i]->bind_group_layout;

    WGPUPipelineLayout wgpu_pipeline_layout = wgpuDeviceCreatePipelineLayout(context->device, &(WGPUPipelineLayoutDescriptor){
        .bindGroupLayouts       = bind_group_layouts,
        .bindGroupLayoutCount   = count,
        .label                  = NULL,
        .nextInChain            = NULL,
    });

    WGPURenderPipeline wgpu_pipeline = wgpuDeviceCreateRenderPipeline(context->device, &(WGPURenderPipelineDescriptor){
        .label = "Render pipeline",
        .layout = wgpu_pipeline_layout,
        .vertex = (WGPUVertexState){
            .module         = shader->module,
            .entryPoint     = "vs_main",
            .bufferCount    = vertex_buffer == NULL ? 0 : 1,
            .buffers        = &vertex_buffer->layout,
        },
        .primitive = (WGPUPrimitiveState){
            .topology           = WGPUPrimitiveTopology_TriangleList,
            .stripIndexFormat   = WGPUIndexFormat_Undefined,
            .frontFace          = WGPUFrontFace_CW,
            .cullMode           = WGPUCullMode_Back,
        },
        .multisample = (WGPUMultisampleState){ 
            .count = 1,
            .mask = ~0,
            .alphaToCoverageEnabled = false,
        },
        .fragment = &(WGPUFragmentState){
            .module         = shader->module,
            .entryPoint     = "fs_main",
            .targetCount    = 1,
            .targets        = &(WGPUColorTargetState){
                .format = context->swapchain_format,
                .blend  = &(WGPUBlendState){
                    .color = (WGPUBlendComponent){
                        .srcFactor = WGPUBlendFactor_One,
                        .dstFactor = WGPUBlendFactor_Zero,
                        .operation = WGPUBlendOperation_Add,
                    },
                    .alpha = (WGPUBlendComponent){
                        .srcFactor = WGPUBlendFactor_One,
                        .dstFactor = WGPUBlendFactor_Zero,
                        .operation = WGPUBlendOperation_Add,
                    }
                },
                .writeMask = WGPUColorWriteMask_All
            },
        },
        .depthStencil = NULL,
    });

    free(vertex_buffer->attributes);

    *render_pipeline = (RenderPipeline)malloc(sizeof(RenderPipeline_s));
    *(*render_pipeline) = (RenderPipeline_s) {
        .render_pipeline = wgpu_pipeline,
    };
}

void render_pipeline_fini(RenderPipeline* render_pipeline) {
    free(*render_pipeline);
}