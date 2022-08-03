#ifndef SAITAMA_ATOMIC_CORE_MATERIAL_H
#define SAITAMA_ATOMIC_CORE_MATERIAL_H

#include "renderer.h"

typedef struct Material_t* Material;

typedef struct MaterialDescriptor {
    const char* shader_path;
} MaterialDescriptor;

int init_material(Material* material, Renderer* renderer, const MaterialDescriptor* descriptor);
int fini_material(Material* material);

#endif