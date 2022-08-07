#include "uniform.h"
#include "uniform_private.h"

#include "../../context.h"
#include "../../context_private.h"

#include "../../buffer.h"
#include "../../buffer_private.h"

extern AtomicContext context;

void uniform_group_init(UniformGroup* uniform_group, const UniformDescription* uniforms, const uint32_t count) {
    WGPUBindGroupLayoutEntry* layout_entries = (WGPUBindGroupLayoutEntry*)malloc(sizeof(WGPUBindGroupLayoutEntry) * count);
    WGPUBindGroupEntry* entries = (WGPUBindGroupEntry*)malloc(sizeof(WGPUBindGroupEntry) * count);

    for (uint32_t i = 0; i < count; i++) {
        WGPUBindGroupLayoutEntry layout_entry = (WGPUBindGroupLayoutEntry) {
            .binding        = i,
            .buffer         = (WGPUBufferBindingLayout) {
                .hasDynamicOffset   = false,
                .nextInChain        = NULL,
                .type               = (WGPUBufferBindingType)uniforms[i].type,
                // .minBindingSize     = uniforms[i].size,
            },
            .sampler        = NULL,
            .storageTexture = NULL,
            .texture        = NULL,
            .visibility     = (WGPUShaderStage)uniforms[i].visibility,
        };

        WGPUBindGroupEntry entry = (WGPUBindGroupEntry) {
            .binding        = i,
            .offset         = uniforms[i].offset,
            
            // This will align in power of two (https://sotrh.github.io/learn-wgpu/showcase/alignment/#alignment-of-vertex-and-index-buffers)
            .size           = (uniforms[i].size + 8 - 1) & ~(8 - 1),

            .buffer         = uniforms[i].buffer->buffer,
            .sampler        = NULL,
            .textureView    = NULL,
            .nextInChain    = NULL,
        };

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