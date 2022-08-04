#ifndef SAITAMA_ATOMIC_CONTEXT_H
#define SAITAMA_ATOMIC_CONTEXT_H

#include "surface/surface.h"

/**
 * An opaque pointer that represent the
 * main context of the application.
 */
typedef struct AtomicContext_s* AtomicContext;

/**
 * @brief Initialize the atomic renderer.
 * 
 * @param surface The surface on wich we render on.
 */
void atm_init(Surface surface);

/**
 * @brief Cleanup atomic renderer ressources.
 * 
 */
void atm_fini();

void atm_begin_render();
void atm_end_render();

#endif