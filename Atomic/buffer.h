#ifndef SAITAMA_ATOMIC_BUFFER_H
#define SAITAMA_ATOMIC_BUFFER_H

#include <stdlib.h>

typedef struct Buffer_s* Buffer;

typedef enum BufferUsage {
    BufferUsage_MapRead         = 0x00000001,
    BufferUsage_MapWrite        = 0x00000002,
    BufferUsage_CopySrc         = 0x00000004,
    BufferUsage_CopyDst         = 0x00000008,
    BufferUsage_Index           = 0x00000010,
    BufferUsage_Vertex          = 0x00000020,
    BufferUsage_Uniform         = 0x00000040,
    BufferUsage_Storage         = 0x00000080,
    BufferUsage_Indirect        = 0x00000100,
    BufferUsage_QueryResolve    = 0x00000200,
} BufferUsage;

typedef enum BufferMapMode {
    BufferMapMode_Read,
    BufferMapMode_Write,
} BufferMapMode;

/**
 * Create a new buffer.
 * 
 * @param buffer    A pointer to the buffer to initialize.
 * @param size      The size (in bytes) of the buffer.
 * @param usage     The buffer usage flag.
 */
void buffer_init(Buffer* buffer, uint64_t size, BufferUsage usage);

/**
 * Create a new buffer and initialize it with data.
 * 
 * @param buffer        A pointer to the buffer to initialize.
 * @param buffer_size   The size (in bytes) of the buffer.
 * @param data          The data to be copied to the buffer.
 * @param data_size     The size (in bytes) of the data.
 * @param usage         The buffer usage flag.
 */
void buffer_init_with_data(Buffer* buffer, uint64_t buffer_size, void* data, uint64_t data_size, BufferUsage usage);
void buffer_fini(Buffer* buffer);

#endif