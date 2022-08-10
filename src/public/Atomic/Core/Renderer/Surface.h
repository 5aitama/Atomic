#ifndef ATOMIC_SURFACE_H
#define ATOMIC_SURFACE_H

#include "../Platform.h"

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
	int surface_init(Surface* surface, const uint32_t width, const uint32_t height, const char* title);

#elif WGPU_TARGET == WGPU_TARGET_IOS
	/**
	* Initialize a new surface.
	* 
	* @param surface		- A pointer to the surface to initialize.
	* @param metal_layer	- A pointer to the MTLLayer object.
	*/
	int surface_init(Surface* surface, void* metal_layer);
#endif

/**
* Destroy properly a surface.
* 
* @param surface - The surface to destroy.
*/
void surface_fini(Surface* surface);

#if WGPU_DESKTOP
	/**
	 * @brief Check if the window should close.
	 * 
	 * @param surface The surface.
	 */
	int surface_window_should_close(Surface surface);

	float surface_window_get_time();
#endif

#endif