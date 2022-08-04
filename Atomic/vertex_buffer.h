#ifndef SAITAMA_ATOMIC_VERTEX_BUFFER_H
#define SAITAMA_ATOMIC_VERTEX_BUFFER_H

#include <stdlib.h>

typedef enum DataFormat {
    DataFormat_Uint8x2    = 0x00000001,
    DataFormat_Uint8x4    = 0x00000002,
    DataFormat_Sint8x2    = 0x00000003,
    DataFormat_Sint8x4    = 0x00000004,
    DataFormat_Unorm8x2   = 0x00000005,
    DataFormat_Unorm8x4   = 0x00000006,
    DataFormat_Snorm8x2   = 0x00000007,
    DataFormat_Snorm8x4   = 0x00000008,
    DataFormat_Uint16x2   = 0x00000009,
    DataFormat_Uint16x4   = 0x0000000A,
    DataFormat_Sint16x2   = 0x0000000B,
    DataFormat_Sint16x4   = 0x0000000C,
    DataFormat_Unorm16x2  = 0x0000000D,
    DataFormat_Unorm16x4  = 0x0000000E,
    DataFormat_Snorm16x2  = 0x0000000F,
    DataFormat_Snorm16x4  = 0x00000010,
    DataFormat_Float16x2  = 0x00000011,
    DataFormat_Float16x4  = 0x00000012,
    DataFormat_Float32    = 0x00000013,
    DataFormat_Float32x2  = 0x00000014,
    DataFormat_Float32x3  = 0x00000015,
    DataFormat_Float32x4  = 0x00000016,
    DataFormat_Uint32     = 0x00000017,
    DataFormat_Uint32x2   = 0x00000018,
    DataFormat_Uint32x3   = 0x00000019,
    DataFormat_Uint32x4   = 0x0000001A,
    DataFormat_Sint32     = 0x0000001B,
    DataFormat_Sint32x2   = 0x0000001C,
    DataFormat_Sint32x3   = 0x0000001D,
    DataFormat_Sint32x4   = 0x0000001E,
} DataFormat;

typedef struct BufferLayout {
    const DataFormat* layout;
    const uint32_t count;
} BufferLayout;

typedef struct VertexBuffer_s* VertexBuffer;
  
void vertex_buffer_init(VertexBuffer* vertex_buffer, const uint64_t count, void* data, const BufferLayout attributes);
void vertex_buffer_fini(VertexBuffer* vertex_buffer);

#endif