#include "Atomic/shader.h"
#include "shader_private.h"

#include "Atomic/context.h"
#include "../context/context_private.h"

#include <stdlib.h>
#include <stdio.h>

extern AtomicContext context;

int compile_shader(const char* path, WGPUShaderModule* module) {
    FILE *file = fopen(path, "rb");
    
    if (!file) {
        printf("Failed to compile shader: Unable to open %s\n", path);
        return 0;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    unsigned char *bytes = (unsigned char*)malloc(length + 1);
    fseek(file, 0, SEEK_SET);
    fread(bytes, 1, length, file);
    fclose(file);
    bytes[length] = 0;
    
    *module = wgpuDeviceCreateShaderModule(context->device, &(WGPUShaderModuleDescriptor){
        .nextInChain = (const WGPUChainedStruct*)&(WGPUShaderModuleWGSLDescriptor) {
            .chain = (WGPUChainedStruct) {
                .next = NULL,
                .sType = WGPUSType_ShaderModuleWGSLDescriptor,
            },
            .code = (const char*)bytes,
        },
        .label = path,
    });

    free(bytes);
    return 1;
}

int shader_init(Shader* shader, const char* path) {
    *shader = (Shader)malloc(sizeof(Shader_s));
    return compile_shader(path, &(*shader)->module);
}

void shader_fini(Shader* shader) {
    wgpuShaderModuleDrop((*shader)->module);
    free(*shader);
}