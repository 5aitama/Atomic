#ifndef ATOMIC_CORE_RENDERER_RENDER_PASS_H
#define ATOMIC_CORE_RENDERER_RENDER_PASS_H

typedef struct RenderPass_s* RenderPass;

void render_pass_begin(RenderPass pass);
void render_pass_end(RenderPass pass);

void render_pass_add_color_attachment(RenderPass pass, const float r, const float g, const float b, const float a);

#endif