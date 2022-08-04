#include "material.h"
#include "renderer_private.h"

void compile_shader(const char* path, WGPUShaderModule* module) {
    
}

void delete_shader(WGPUShaderModule module) {

}

int init_material(Material* material, Renderer* renderer, const MaterialDescriptor* descriptor) {

    WGPUPipelineLayout wgpu_pipeline_layout = wgpuDeviceCreatePipelineLayout((*renderer)->device, &(WGPUPipelineLayoutDescriptor){
        .bindGroupLayouts       = NULL,
        .bindGroupLayoutCount   = 0,
    });

    WGPURenderPipeline wgpu_pipeline = wgpuDeviceCreateRenderPipeline((*renderer)->device, &(WGPURenderPipelineDescriptor){
        .label = "Render pipeline",
        .layout = wgpu_pipeline_layout,
        .vertex = (WGPUVertexState){
            .module         = shader,
            .entryPoint     = "vs_main",
            .bufferCount    = 0,
            .buffers        = NULL,
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
            .module         = shader,
            .entryPoint     = "fs_main",
            .targetCount    = 1,
            .targets        = &(WGPUColorTargetState){
                .format = (*renderer)->swapchain_format,
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
    return EXIT_SUCCESS;
}

int fini_material(Material* material) {
    return EXIT_SUCCESS;
}