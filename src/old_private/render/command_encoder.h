#ifndef SAITAMA_ATOMIC_COMMAND_ENCODER_PRIVATE_H
#define SAITAMA_ATOMIC_COMMAND_ENCODER_PRIVATE_H

#include <wgpu.h>

/**
 * Encodes a series of GPU operations.
 * 
 * A command encoder can record RenderPasses, ComputePasses, 
 * and transfer operations between driver-managed resources
 * like Buffers and Textures.
 * 
 */
typedef struct CommandEncoder_s {
    WGPUCommandEncoder encoder;
} CommandEncoder_s;

#endif