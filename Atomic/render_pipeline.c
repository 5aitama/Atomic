#include "render_pipeline.h"
#include "render_pipeline_private.h"

#include "context.h"
#include "context_private.h"

#include "shader_private.h"
#include "vertex_buffer_private.h"

extern AtomicContext context;

void render_pipeline_init(RenderPipeline* render_pipeline, Shader shader, VertexBuffer vertex_buffer) {

    WGPUBindGroupLayoutEntry vertex_entry = (WGPUBindGroupLayoutEntry) {
        .binding        = 0,
        .buffer         = (WGPUBufferBindingLayout) {
            .hasDynamicOffset   = false,
            .nextInChain        = NULL,
            .type               = WGPUBufferBindingType_Uniform,
            .minBindingSize     = 0,
        },
        .sampler        = NULL,
        .storageTexture = NULL,
        .texture        = NULL,
        .visibility     = WGPUShaderStage_Vertex,
    };

    WGPUBindGroupLayout wgpu_bind_group_layout = wgpuDeviceCreateBindGroupLayout(context->device, &(WGPUBindGroupLayoutDescriptor) {
        .entries        = &vertex_entry,
        .entryCount     = 1,
        .nextInChain    = NULL,
        .label          = NULL,
    });

    WGPUPipelineLayout wgpu_pipeline_layout = wgpuDeviceCreatePipelineLayout(context->device, &(WGPUPipelineLayoutDescriptor){
        .bindGroupLayouts       = NULL,//&wgpu_bind_group_layout,
        .bindGroupLayoutCount   = 0,
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
            .frontFace          = WGPUFrontFace_CCW,
            .cullMode           = WGPUCullMode_None
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

    *render_pipeline = (RenderPipeline)malloc(sizeof(RenderPipeline_s));
    *(*render_pipeline) = (RenderPipeline_s) {
        .render_pipeline = wgpu_pipeline,
    };
}

void render_pipeline_fini(RenderPipeline* render_pipeline) {
    free(*render_pipeline);
}