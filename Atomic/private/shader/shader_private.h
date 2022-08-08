#ifndef SAITAMA_ATOMIC_SHADER_PRIVATE_H
#define SAITAMA_ATOMIC_SHADER_PRIVATE_H

#include <wgpu.h>

typedef struct Shader_s {
    WGPUShaderModule module;
} Shader_s;

#endif