#ifndef ATOMIC_CORE_RENDERER_CONTEXT_H
#define ATOMIC_CORE_RENDERER_CONTEXT_H

#include <Atomic/Core/Renderer/Surface.h>

/**
 * @brief Initialize the Atomic Renderer.
 * This function must and can be called
 * once during the application lifetime.
 * 
 * @param surface The surface on wich the renderer will render.
 */
void renderer_init(Surface surface);

/**
 * @brief Destroy gracefully the Atomic
 * Renderer. This function must be called
 * once after `renderer_init()` and before
 * the application close.
 * 
 */
void renderer_fini();

void renderer_aquire_texture();
void renderer_present();

#endif