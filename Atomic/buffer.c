#include "buffer.h"
#include "buffer_private.h"

#include "context.h"
#include "context_private.h"

#include <math.h>

extern AtomicContext context;

#define WGPU_COPY_BUFFER_ALIGNMENT 32

void buffer_init(Buffer* buffer, uint64_t size, BufferUsage usage) {
    uint64_t buffer_align_mask  = WGPU_COPY_BUFFER_ALIGNMENT - 1;
    uint64_t buffer_padded_size = fmax((size + buffer_align_mask) & ~buffer_align_mask, WGPU_COPY_BUFFER_ALIGNMENT);

    WGPUBuffer wgpu_buffer = wgpuDeviceCreateBuffer(context->device, &(WGPUBufferDescriptor) {
        .label              = NULL,
        .mappedAtCreation   = 0,
        .size               = buffer_padded_size,
        .usage              = (WGPUBufferUsage)usage,
    });

    *buffer = (Buffer)malloc(sizeof(Buffer_s));
    *(*buffer) = (Buffer_s) {
        .buffer = wgpu_buffer,
        .size   = buffer_padded_size,
    };
}

void buffer_init_with_data(Buffer* buffer, uint64_t buffer_size, void* data, uint64_t data_size, BufferUsage usage) {
    uint64_t buffer_align_mask  = WGPU_COPY_BUFFER_ALIGNMENT - 1;
    uint64_t buffer_padded_size = fmax((buffer_size + buffer_align_mask) & ~buffer_align_mask, WGPU_COPY_BUFFER_ALIGNMENT);

    WGPUBuffer wgpu_buffer = wgpuDeviceCreateBuffer(context->device, &(WGPUBufferDescriptor) {
        .label              = NULL,
        .mappedAtCreation   = 1,
        .size               = buffer_padded_size,
        .usage              = (WGPUBufferUsage)usage,
    });

    memcpy(wgpuBufferGetMappedRange(wgpu_buffer, 0, buffer_padded_size), data, data_size);
    wgpuBufferUnmap(wgpu_buffer);

    *buffer = (Buffer)malloc(sizeof(Buffer_s));
    *(*buffer) = (Buffer_s) {
        .buffer = wgpu_buffer,
        .size   = buffer_padded_size,
    };
}

void buffer_map(Buffer buffer, BufferMapMode mode, size_t offset, size_t size) {
    // TODO: Finish the map buffer function. This will need to take a callback
    // function as an argument because it's async... To wait it you need to call
    // wgpuDevicePoll() in the render loop.

    // wgpuBufferMapAsync(buffer->buffer, (WGPUMapMode)mode, offset, size, wgpu_buffer_map_callback, );
}

void buffer_fini(Buffer* buffer) {
    wgpuBufferDrop((*buffer)->buffer);
    free(*buffer);
}