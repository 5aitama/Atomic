#ifndef SAITAMA_ATOMIC_VERTEX_BUFFER_PRIVATE_H
#define SAITAMA_ATOMIC_VERTEX_BUFFER_PRIVATE_H

#include <wgpu.h>
#include "buffer.h"

typedef struct VertexBuffer_s {
    WGPUVertexAttribute*    attributes;
    WGPUVertexBufferLayout  layout;
    Buffer                  buffer;
} VertexBuffer_s;

#endif