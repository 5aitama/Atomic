#ifndef SAITAMA_ATOMIC_UNIFORM_PRIVATE_H
#define SAITAMA_ATOMIC_UNIFORM_PRIVATE_H

#include <wgpu.h>
#include <Atomic/buffer.h>
#include "../buffer_private.h"

typedef struct UniformGroup_s {
    WGPUBindGroup       bind_group;
    WGPUBindGroupLayout bind_group_layout;
} UniformGroup_s;

#endif