#ifndef SAITAMA_ATOMIC_SAMPLER_H
#define SAITAMA_ATOMIC_SAMPLER_H

typedef struct Sampler_t* Sampler;

void sampler_init(Sampler* sampler);
void sampler_fini(Sampler* sampler);

#endif