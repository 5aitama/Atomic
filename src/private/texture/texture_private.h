#ifndef SAITAMA_ATOMIC_TEXTURE_PRIVATE_H
#define SAITAMA_ATOMIC_TEXTURE_PRIVATE_H

#include <wgpu.h>

typedef struct Texture2D_t {
    WGPUTexture texture;
    WGPUTextureView texture_view;
} Texture2D_t;

#endif