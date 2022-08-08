#ifndef SAITAMA_ATOMIC_SHADER_H
#define SAITAMA_ATOMIC_SHADER_H

/**
 * An opaque pointer to the shader object.
 */
typedef struct Shader_s* Shader;

/**
 * @brief Create a new shader.
 * 
 * @param shader A pointer to the shader to initialize.
 * @param path The path of the shader source file (wgsl).
 */
int shader_init(Shader* shader, const char* path);

void shader_fini(Shader* shader);

#endif