#include "vertex_buffer.h"
#include "vertex_buffer_private.h"

#include "context.h"
#include "context_private.h"

#include "buffer.h"
#include "buffer_private.h"

#include <math.h>
#include <stdio.h>

extern AtomicContext context;

size_t size_of_data_format(const DataFormat& format) {
    switch (format) {
        case DataFormat_Uint8x2   : return sizeof(uint8_t)  * 2;
        case DataFormat_Uint8x4   : return sizeof(uint8_t)  * 4;
        case DataFormat_Sint8x2   : return sizeof(int8_t)   * 2;
        case DataFormat_Sint8x4   : return sizeof(int8_t)   * 4;
        case DataFormat_Unorm8x2  : return sizeof(uint8_t)  * 2;
        case DataFormat_Unorm8x4  : return sizeof(uint8_t)  * 4;
        case DataFormat_Snorm8x2  : return sizeof(int8_t)   * 2;
        case DataFormat_Snorm8x4  : return sizeof(int8_t)   * 4;
        case DataFormat_Uint16x2  : return sizeof(uint16_t) * 2;
        case DataFormat_Uint16x4  : return sizeof(uint16_t) * 4;
        case DataFormat_Sint16x2  : return sizeof(int16_t)  * 2;
        case DataFormat_Sint16x4  : return sizeof(int16_t)  * 4;
        case DataFormat_Unorm16x2 : return sizeof(uint16_t) * 2;
        case DataFormat_Unorm16x4 : return sizeof(uint16_t) * 4;
        case DataFormat_Snorm16x2 : return sizeof(int16_t)  * 2;
        case DataFormat_Snorm16x4 : return sizeof(int16_t)  * 4;
        case DataFormat_Float16x2 : return sizeof(float)       ;
        case DataFormat_Float16x4 : return sizeof(float)    * 2;
        case DataFormat_Float32   : return sizeof(float)       ;
        case DataFormat_Float32x2 : return sizeof(float)    * 2;
        case DataFormat_Float32x3 : return sizeof(float)    * 3;
        case DataFormat_Float32x4 : return sizeof(float)    * 4;
        case DataFormat_Uint32    : return sizeof(uint32_t)    ;
        case DataFormat_Uint32x2  : return sizeof(uint32_t) * 2;
        case DataFormat_Uint32x3  : return sizeof(uint32_t) * 3;
        case DataFormat_Uint32x4  : return sizeof(uint32_t) * 4;
        case DataFormat_Sint32    : return sizeof(int32_t)     ;
        case DataFormat_Sint32x2  : return sizeof(int32_t)  * 2;
        case DataFormat_Sint32x3  : return sizeof(int32_t)  * 3;
        case DataFormat_Sint32x4  : return sizeof(int32_t)  * 4;
    }
}

WGPUVertexFormat data_format_into_wgpu_vertex_format(const DataFormat& format) {
    switch (format) {
        case DataFormat_Uint8x2    : return WGPUVertexFormat_Uint8x2   ;
        case DataFormat_Uint8x4    : return WGPUVertexFormat_Uint8x4   ;
        case DataFormat_Sint8x2    : return WGPUVertexFormat_Sint8x2   ;
        case DataFormat_Sint8x4    : return WGPUVertexFormat_Sint8x4   ;
        case DataFormat_Unorm8x2   : return WGPUVertexFormat_Unorm8x2  ;
        case DataFormat_Unorm8x4   : return WGPUVertexFormat_Unorm8x4  ;
        case DataFormat_Snorm8x2   : return WGPUVertexFormat_Snorm8x2  ;
        case DataFormat_Snorm8x4   : return WGPUVertexFormat_Snorm8x4  ;
        case DataFormat_Uint16x2   : return WGPUVertexFormat_Uint16x2  ;
        case DataFormat_Uint16x4   : return WGPUVertexFormat_Uint16x4  ;
        case DataFormat_Sint16x2   : return WGPUVertexFormat_Sint16x2  ;
        case DataFormat_Sint16x4   : return WGPUVertexFormat_Sint16x4  ;
        case DataFormat_Unorm16x2  : return WGPUVertexFormat_Unorm16x2 ;
        case DataFormat_Unorm16x4  : return WGPUVertexFormat_Unorm16x4 ;
        case DataFormat_Snorm16x2  : return WGPUVertexFormat_Snorm16x2 ;
        case DataFormat_Snorm16x4  : return WGPUVertexFormat_Snorm16x4 ;
        case DataFormat_Float16x2  : return WGPUVertexFormat_Float16x2 ; 
        case DataFormat_Float16x4  : return WGPUVertexFormat_Float16x4 ;
        case DataFormat_Float32    : return WGPUVertexFormat_Float32   ;
        case DataFormat_Float32x2  : return WGPUVertexFormat_Float32x2 ;
        case DataFormat_Float32x3  : return WGPUVertexFormat_Float32x3 ;
        case DataFormat_Float32x4  : return WGPUVertexFormat_Float32x4 ;
        case DataFormat_Uint32     : return WGPUVertexFormat_Uint32    ;
        case DataFormat_Uint32x2   : return WGPUVertexFormat_Uint32x2  ;
        case DataFormat_Uint32x3   : return WGPUVertexFormat_Uint32x3  ;
        case DataFormat_Uint32x4   : return WGPUVertexFormat_Uint32x4  ;
        case DataFormat_Sint32     : return WGPUVertexFormat_Sint32    ;
        case DataFormat_Sint32x2   : return WGPUVertexFormat_Sint32x2  ;
        case DataFormat_Sint32x3   : return WGPUVertexFormat_Sint32x3  ;
        case DataFormat_Sint32x4   : return WGPUVertexFormat_Sint32x4  ;
    }
}

void vertex_buffer_init(VertexBuffer* vertex_buffer, const uint64_t count, void* data, const BufferLayout attributes) {
    uint64_t stride = 0;

    WGPUVertexAttribute* wgpu_vertex_attributes = (WGPUVertexAttribute*)malloc(sizeof(WGPUVertexAttribute) * attributes.count);

    for (uint32_t i = 0; i < attributes.count; i++) {
        wgpu_vertex_attributes[i] = (WGPUVertexAttribute) {
            .format         = data_format_into_wgpu_vertex_format(attributes.layout[i]),
            .offset         = stride,
            .shaderLocation = i,
        };
        
        stride += size_of_data_format(attributes.layout[i]);
    }

    uint64_t total_size = stride * count;

    WGPUVertexBufferLayout wgpu_vertex_buffer_layout = (WGPUVertexBufferLayout) {
        .arrayStride    = stride,
        .attributeCount = attributes.count,
        .attributes     = wgpu_vertex_attributes,
        .stepMode       = WGPUVertexStepMode_Vertex,
    };

    Buffer vbuff = NULL;
    buffer_init_with_data(&vbuff, total_size, data, total_size, BufferUsage_Vertex);

    *vertex_buffer = (VertexBuffer)malloc(sizeof(VertexBuffer_s));
    *(*vertex_buffer) = (VertexBuffer_s) {
        .layout         = wgpu_vertex_buffer_layout,
        .attributes     = wgpu_vertex_attributes,
        .buffer         = vbuff,
    };
}

void vertex_buffer_fini(VertexBuffer* vertex_buffer) {
    buffer_fini(&(*vertex_buffer)->buffer);
    free(*vertex_buffer);
}