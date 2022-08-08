#ifndef SAITAMA_ATOMIC_RENDER_PASS_H
#define SAITAMA_ATOMIC_RENDER_PASS_H

#include <Atomic/render_pipeline.h>
#include <Atomic/vertex_buffer.h>
#include <Atomic/uniform.h>

#include <stdlib.h>

typedef struct RenderPass_s* RenderPass;
typedef enum IndexFormat {
    IndexFormat_uint16 = 0x00000001,
    IndexFormat_uint32 = 0x00000002,
} IndexFormat;

void render_pass_init(RenderPass* render_pass);
void render_pass_fini(RenderPass* render_pass);

void render_pass_begin(RenderPass render_pass);
void render_pass_end(RenderPass render_pass);

void render_pass_set_pipeline(RenderPass render_pass, RenderPipeline render_pipeline);

void render_pass_draw(
    RenderPass render_pass, 
    const uint32_t vertex_count, 
    const uint32_t instance_count,
    const uint32_t first_vertex, 
    const uint32_t first_instance
);

void render_pass_draw_indexed(
    RenderPass render_pass,
    const uint32_t index_count,
    const uint32_t instance_count,
    const uint32_t first_index,
    const uint32_t base_vertex,
    const uint32_t first_instance
);

void render_pass_set_index_buffer(RenderPass render_pass, Buffer index_buffer, const IndexFormat format, const uint64_t offset, const uint64_t size);
void render_pass_set_vertex_buffer(RenderPass render_pass, VertexBuffer vertex_buffer);
void render_pass_set_uniform_groups(RenderPass render_pass, UniformGroup* uniform_groups, const uint32_t count);
#endif