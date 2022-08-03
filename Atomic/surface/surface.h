#ifndef ATOMIC_SURFACE_H
#define ATOMIC_SURFACE_H
#include "../Atomic.h"

#if WGPU_DESKTOP
	#include <GLFW/glfw3.h>
	#if   WGPU_TARGET == WGPU_TARGET_MACOS
		#define GLFW_EXPOSE_NATIVE_COCOA
	#elif WGPU_TARGET == WGPU_TARGET_LINUX_X11
		#define GLFW_EXPOSE_NATIVE_X11
	#elif WGPU_TARGET == WGPU_TARGET_LINUX_WAYLAND
		#define GLFW_EXPOSE_NATIVE_WAYLAND
	#elif WGPU_TARGET == WGPU_TARGET_WINDOWS
		#define GLFW_EXPOSE_NATIVE_WIN32
	#endif
	#include <GLFW/glfw3native.h>
#endif

/**
* Represent a surface on wich we can draw on.
*/
typedef struct Surface_t* Surface;

#if WGPU_DESKTOP
	/**
	* Initialize a new surface.
	* 
	* @param surface	- A pointer to the surface to initialize.
	* @param width		- The window width.
	* @param height		- The window height.
	* @param title		- The window title.
	*/
	int init_surface(Surface* surface, const uint32_t width, const uint32_t height, const char* title);

	/**
	* Get the pointer to the GLFWWindow of the surface.
	* @param surface - The targeted surface.
	*/
	GLFWwindow* surface_get_glfw_window(Surface* surface);

#elif WGPU_TARGET == WGPU_TARGET_IOS
	/**
	* Initialize a new surface.
	* 
	* @param surface		- A pointer to the surface to initialize.
	* @param metal_layer	- A pointer to the MTLLayer object.
	*/
	int init_surface(Surface* surface, void* metal_layer);
#endif

/**
* Destroy properly a surface.
* 
* @param surface - The surface to destroy.
*/
void fini_surface(Surface* surface);

/**
 * Get the raw pointer of the surface.
 * 
 * @param surface The surface.
 */
void* surface_get_raw(Surface* surface);

void surface_get_size(Surface* surface, uint32_t* width, uint32_t* height);

#endif