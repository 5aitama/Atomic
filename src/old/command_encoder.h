#ifndef SAITAMA_ATOMIC_COMMAND_ENCODER_H
#define SAITAMA_ATOMIC_COMMAND_ENCODER_H

typedef struct CommandEncoder_s* CommandEncoder;

/**
 * Create a new CommandEncoder.
 * 
 * @param command_encoder A pointer to the command encoder object to initialize.
 */
void command_encoder_init(CommandEncoder* command_encoder);

/**
 * Destroy a CommandEncoder.
 * 
 * @param command_encoder A pointer to the command encoder to destroy.
 */
void command_encoder_fini(CommandEncoder* command_encoder);

#endif