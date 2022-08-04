#ifndef SAITAMA_ATOMIC_BUFFER_PRIVATE_H
#define SAITAMA_ATOMIC_BUFFER_PRIVATE_H

#include <wgpu.h>
#include <stdlib.h>

typedef struct Buffer_s {
    WGPUBuffer  buffer;
    uint64_t    size;
} Buffer_s;

#endif