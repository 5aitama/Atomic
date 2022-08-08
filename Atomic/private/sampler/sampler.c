#include "Atomic/sampler.h"
#include "sampler_private.h"

#include "Atomic/context.h"
#include "../context/context_private.h"

extern AtomicContext context;

void sampler_init(Sampler* sampler) {
    WGPUSampler wgpu_sampler = wgpuDeviceCreateSampler(context->device, &(WGPUSamplerDescriptor){
        .addressModeU   = WGPUAddressMode_Repeat,
        .addressModeV   = WGPUAddressMode_Repeat,
        .addressModeW   = WGPUAddressMode_Repeat,
        .compare        = WGPUCompareFunction_Undefined,
        .label          = NULL,
        .lodMaxClamp    = 0,
        .lodMinClamp    = 0,
        .magFilter      = WGPUFilterMode_Linear,
        .maxAnisotropy  = 1,
        .minFilter      = WGPUFilterMode_Linear,
        .mipmapFilter   = WGPUMipmapFilterMode_Linear,
        .nextInChain    = NULL,
    });

    *sampler = (Sampler)malloc(sizeof(Sampler_t));
    *(*sampler) = (Sampler_t){
        .sampler = wgpu_sampler,
    };
}

void sampler_fini(Sampler* sampler) {
    wgpuSamplerDrop((*sampler)->sampler);
    free(*sampler);
}