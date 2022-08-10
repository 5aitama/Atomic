#ifndef SAITAMA_ATOMIC_TEXTURE_H
#define SAITAMA_ATOMIC_TEXTURE_H

typedef struct Texture2D_t* Texture2D;

/**
 * @brief Create a new 2D Texture.
 * 
 * @param texture2d A pointer to the 2D texture to initialize.
 * @param path The texture image path.
 */
void texture2D_init(Texture2D* texture2d, const char* path);

/**
 * @brief Destroy a 2D Texture.
 * 
 * @param texture2d A pointer to the 2D texture to destroy.
 */
void texture2D_fini(Texture2D* texture2d);

#endif