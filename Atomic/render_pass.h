#ifndef SAITAMA_ATOMIC_RENDER_PASS_H
#define SAITAMA_ATOMIC_RENDER_PASS_H

#include "render_pipeline.h"
#include "vertex_buffer.h"

#include <stdlib.h>

typedef struct RenderPass_s* RenderPass;

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

void render_pass_set_vertex_buffer(RenderPass render_pass, VertexBuffer vertex_buffer);

#endif