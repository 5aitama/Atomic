#ifndef ATOMIC_CORE_RENDERER_PASS_ENCODER_H
#define ATOMIC_CORE_RENDERER_PASS_ENCODER_H

#include <stdint.h>

typedef struct PassEncoder_s* PassEncoder;

typedef enum PassType {
    PassType_RenderPass,
    PassType_ComputePass,
} PassType;

typedef void (*PassFunc)(void*, void*);

typedef struct PassDescriptor {
    PassType type;
    PassFunc func;
} PassDescriptor;

// typedef void (*PassFunctions)(PassEncoder encoder, void*);

/**
 * @brief Create a new PassEncoder.
 * 
 * @param encoder A pointer to the PassEncoder to initialize.
 */
void pass_encoder_init(PassEncoder* encoder);

/**
 * @brief Destroy a PassEncoder.
 * 
 * @param encoder A pointer to the PassEncoder to destroy.
 */
void pass_encoder_fini(PassEncoder* encoder);

/**
 * @brief Encode a list of passes.
 * 
 * @param encoder The pass encoder on wich the passes will be set.
 * @param passes The passes to be encoded.
 * @param count The amount of passes to encode.
 */
void pass_encoder_encode_passes(PassEncoder encoder, PassDescriptor* passes, const uint32_t count, void* user_data);

/**
 * @brief Submit all encoded passes to the GPU to execute them.
 * 
 * @param encoder The pass encoders to submit.
 * @param count The amount of the given pass encoders.
 */
void pass_encoder_submit(PassEncoder* encoders, const uint32_t count);
#endif