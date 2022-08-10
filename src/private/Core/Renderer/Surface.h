#ifndef ATOMIC_CORE_RENDERER_SURFACE_PRIVATE_H
#define ATOMIC_CORE_RENDERER_SURFACE_PRIVATE_H

#include <Atomic/Core/Platform.h>
#include <wgpu.h>

#if WGPU_TARGET == WGPU_TARGET_WINDOWS
    #include <Windows.h>
#elif WGPU_TARGET == WGPU_TARGET_MACOS
    #include <Foundation/Foundation.h>
    #include <QuartzCore/CAMetalLayer.h>
#endif

/*
 * Represent a surface on wich we can do
 * rendering operations.
 */
typedef struct Surface_t {
#if WGPU_DESKTOP
	/* The window (only for desktop platforms)*/
	GLFWwindow* window;
#endif
	/*
	The wgpu surface on wich we can render. This
	is available on Windows, macOS, Linux, iOS,
	iPadOS and Android.
	*/
	WGPUSurface surface;
} Surface_t;

/**
 * @brief Get the size of a surface.
 * 
 * @param surface   A pointer to the surface
 * @param width     Where to store the surface width
 * @param height    Where to store the surface height
 */
void surface_get_size(Surface* surface, uint32_t* width, uint32_t* height);

#endif