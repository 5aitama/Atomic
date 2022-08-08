#include "uniform.h"
#include "uniform_private.h"

#include "../../context.h"
#include "../../context_private.h"

#include "../../buffer.h"
#include "../../buffer_private.h"

#include "../../texture/texture_private.h"
#include "../../texture/sampler_private.h"

extern AtomicContext context;

void uniform_group_init(UniformGroup* uniform_group, const UniformDescription* uniforms, const uint32_t count) {
    WGPUBindGroupLayoutEntry* layout_entries = (WGPUBindGroupLayoutEntry*)malloc(sizeof(WGPUBindGroupLayoutEntry) * count);
    WGPUBindGroupEntry* entries = (WGPUBindGroupEntry*)malloc(sizeof(WGPUBindGroupEntry) * count);

    for (uint32_t i = 0; i < count; i++) {
        WGPUBindGroupLayoutEntry layout_entry = (WGPUBindGroupLayoutEntry) {
            .binding        = i,
            .buffer         = NULL,
            .sampler        = NULL,
            .storageTexture = NULL,
            .texture        = NULL,
            .visibility     = (WGPUShaderStage)uniforms[i].visibility,
        };

        WGPUBindGroupEntry entry = (WGPUBindGroupEntry) {
            .binding        = i,
            .offset         = 0,
            .size           = 0,
            .buffer         = NULL,
            .sampler        = NULL,
            .textureView    = NULL,
            .nextInChain    = NULL,
        };

        if (uniforms[i].buffer != NULL) {
            layout_entry.buffer = (WGPUBufferBindingLayout) {
                .hasDynamicOffset   = false,
                .nextInChain        = NULL,
                .type               = (WGPUBufferBindingType)uniforms[i].type,
            };

            entry.buffer    = uniforms[i].buffer->buffer;
            
            // This will align in power of two (https://sotrh.github.io/learn-wgpu/showcase/alignment/#alignment-of-vertex-and-index-buffers)
            entry.size      = (uniforms[i].size + 8 - 1) & ~(8 - 1);
            entry.offset    = 0;

        } else if (uniforms[i].texture != NULL) {
            layout_entry.texture = (WGPUTextureBindingLayout) {
                .multisampled   = false,
                .nextInChain    = NULL,
                .sampleType     = WGPUTextureSampleType_Float,
                .viewDimension  = WGPUTextureViewDimension_2D,
            };

            entry.textureView = uniforms[i].texture->texture_view;
        } else if (uniforms[i].sampler != NULL) {
            layout_entry.sampler = (WGPUSamplerBindingLayout) {
                .type = WGPUSamplerBindingType_Filtering,
                .nextInChain = NULL,
            };

            entry.sampler = uniforms[i].sampler->sampler;
        }

        layout_entries[i] = layout_entry;
        entries[i] = entry;
    }

    WGPUBindGroupLayout bind_group_layout = wgpuDeviceCreateBindGroupLayout(context->device, &(WGPUBindGroupLayoutDescriptor) {
        .entries        = layout_entries,
        .entryCount     = count,
        .nextInChain    = NULL,
        .label          = NULL,
    });

    WGPUBindGroup bind_group = wgpuDeviceCreateBindGroup(context->device, &(WGPUBindGroupDescriptor) {
        .layout     = bind_group_layout,
        .entries    = entries,
        .entryCount = count,
        .label      = NULL,
    });

    free(entries);
    free(layout_entries);

    *uniform_group = (UniformGroup)malloc(sizeof(UniformGroup_s));
    *(*uniform_group) = (UniformGroup_s) {
        .bind_group         = bind_group,
        .bind_group_layout  = bind_group_layout,
    };
}

void uniform_group_fini(UniformGroup* uniform_group) {
    wgpuBindGroupDrop((*uniform_group)->bind_group);
    wgpuBindGroupLayoutDrop((*uniform_group)->bind_group_layout);

    free(*uniform_group);
}