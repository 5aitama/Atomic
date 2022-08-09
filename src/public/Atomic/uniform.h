#ifndef SAITAMA_ATOMIC_UNIFORM_H
#define SAITAMA_ATOMIC_UNIFORM_H

#include <stdlib.h>
#include <stdint.h>
#include <Atomic/buffer.h>
#include <Atomic/texture.h>
#include <Atomic/sampler.h>

typedef struct UniformGroup_s*  UniformGroup;

typedef enum UniformType {
    UniformType_Uniform         = 0x00000001,
    UniformType_Storage         = 0x00000002,
    UniformType_ReadOnlyStorage = 0x00000003,
} UniformType;

typedef enum UniformVisibility {
    UniformVisibility_Vertex    = 0x00000001,
    UniformVisibility_Fragment  = 0x00000002,
    UniformVisibility_Compute   = 0x00000004,
} UniformVisibility;

typedef struct UniformDescription {
    /** The type of the uniform. */
    UniformType type;

    /** The visibility of the uniform */
    UniformVisibility visibility;

    /** The offset (in bytes) in the buffer of the uniform data. */
    uint64_t offset;

    /** The size (in bytes) in the buffer of the uniform data. */
    uint64_t size;

    /** The buffer where the uniform data is. */
    Buffer buffer;

    /** The texture */
    Texture2D texture;

    /** A texture sampler */
    Sampler sampler;
} UniformDescription;

/**
 * @brief Create a new uniform group.
 * 
 * @param uniform_group A pointer to the uniform group to initialize.
 * @param uniforms The description of all uniforms in the group.
 * @param count The amount of uniform in the group.
 */
void uniform_group_init(UniformGroup* uniform_group, const UniformDescription* uniforms, const uint32_t count);
void uniform_group_fini(UniformGroup* uniform_group);

#endif