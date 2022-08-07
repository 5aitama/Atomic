#ifndef SAITAMA_ATOMIC_RENDER_PIPELINE_H
#define SAITAMA_ATOMIC_RENDER_PIPELINE_H

#include <stdlib.h>

#include "shader.h"
#include "vertex_buffer.h"
#include "buffer/uniform/uniform.h"

typedef struct RenderPipeline_s* RenderPipeline;

/**
 * @brief Create a new Render Pipeline.
 * 
 * @param render_pipeline A pointer to the render pipeline to initialize.
 * @param shader The shader used by the pipeline.
 */
void render_pipeline_init(RenderPipeline* render_pipeline, Shader shader, VertexBuffer vertex_buffer, const UniformGroup* uniform_groups, const uint32_t count);
void render_pipeline_fini(RenderPipeline* render_pipeline);

// void render_pipeline_use_shader(RenderPipeline* render_pipeline, Shader shader);
// void render_pipeline_draw(RenderPipeline* render_pipeline);
#endif