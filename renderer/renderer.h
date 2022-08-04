#ifndef SAITAMA_ATOMIC_CORE_RENDERER_H
#define SAITAMA_ATOMIC_CORE_RENDERER_H

#include "../surface/surface.h"

typedef struct Renderer_t* Renderer;

int init_renderer(Renderer* renderer, Surface* surface);
int fini_renderer(Renderer* renderer);
int render_frame(Renderer* renderer);
#endif