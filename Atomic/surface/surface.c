#include "surface.h"
#include <wgpu.h>
#include <stdio.h>
#include <stdlib.h>

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

#if WGPU_DESKTOP
/**
* @brief Initialize a new surface.
*
* @param surface - A pointer to the surface to initialize.
* @param width	 - The window width.
* @param height	 - The window height.
* @param title	 - The window title.
*/
int init_surface(Surface* surface, const uint32_t width, const uint32_t height, const char* title)
#elif WGPU_TARGET == WGPU_TARGET_IOS
/**
* @brief Initialize a new surface.
*
* @param surface - A pointer to the surface to initialize.
* @param mtl_layer - A pointer to the metal layer.
*/
int init_surface(Surface* surface, void* metal_layer)
#endif
{
#if WGPU_DESKTOP
	if (glfwInit() == GLFW_FALSE) {
		fprintf(stderr, "Failed to initialize glfw !\n");
		return EXIT_FAILURE;
	}

	if (width == 0 || height == 0) {
		fprintf(stderr, "The windows width | height must be greater that 0 !\n");
		glfwTerminate();
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* window = glfwCreateWindow((int)width, (int)height, title, NULL, NULL);

	if (window == NULL) {
		fprintf(stderr, "An error occured when create a window !\n");
		glfwTerminate();
		return EXIT_FAILURE;
	}
#endif

#if WGPU_TARGET == WGPU_TARGET_WINDOWS

	WGPUSurfaceDescriptorFromWindowsHWND sdesc = (WGPUSurfaceDescriptorFromWindowsHWND) {
		.hwnd = glfwGetWin32Window(window),
		.hinstance = GetModuleHandle(NULL),
		.chain = (WGPUChainedStruct){
			.next = NULL,
			.sType = WGPUSType_SurfaceDescriptorFromWindowsHWND
		},
	};

#elif WGPU_TARGET == WGPU_TARGET_MACOS
	id metal_layer = NULL;
	NSWindow* ns_window = glfwGetCocoaWindow(window);
	[ns_window.contentView setWantsLayer : YES] ;
	metal_layer = [CAMetalLayer layer];
	[ns_window.contentView setLayer : metal_layer] ;

	WGPUSurfaceDescriptorFromMetalLayer sdesc = (WGPUSurfaceDescriptorFromMetalLayer){
		.layer = metal_layer,
		.chain = (WGPUChainedStruct){
			.next = NULL,
			.sType = WGPUSType_SurfaceDescriptorFromMetalLayer
		},
	};
#elif WGPU_TARGET == WGPU_TARGET_LINUX_X11
	WGPUSurfaceDescriptorFromXlibWindow sdesc = (WGPUSurfaceDescriptorFromXlibWindow){
		.display = glfwGetX11Display(),
		.window = glfwGetX11Window(window),
		.chain = (WGPUChainedStruct){
			.next = NULL,
			.sType = WGPUSType_SurfaceDescriptorFromXlibWindow
		},
	};
#elif WGPU_TARGET == WGPU_TARGET_LINUX_WAYLAND
	WGPUSurfaceDescriptorFromWaylandSurface sdesc = (WGPUSurfaceDescriptorFromWaylandSurface){
		.display = glfwGetWaylandDisplay(),
		.surface = glfwGetWaylandWindow(window),
		.chain = (WGPUChainedStruct){
			.next = NULL,
			.sType = WGPUSType_SurfaceDescriptorFromWaylandSurface
		},
	};
#elif WGPU_TARGET == WGPU_TARGET_IOS
	WGPUSurfaceDescriptorFromMetalLayer sdesc = (WGPUSurfaceDescriptorFromMetalLayer){
		.layer = metal_layer,
		.chain = (WGPUChainedStruct){
			.next = NULL,
			.sType = WGPUSType_SurfaceDescriptorFromMetalLayer
		},
	};
#endif

	WGPUSurfaceDescriptor desc = (WGPUSurfaceDescriptor) {
		.label = NULL,
		.nextInChain = (const WGPUChainedStruct*)&sdesc,
	};
	
	WGPUSurface wgpu_surface = wgpuInstanceCreateSurface(NULL, &desc);
	
	// Don't forget to free it !
	(*surface) = (Surface_t*)malloc(sizeof(Surface_t));

	if ((*surface) == NULL) {
		printf("Failed to allocate memory for create a new surface !\n");
		return EXIT_FAILURE;
	} else {
		*(*surface) = (Surface_t){
			.window = window,
			.surface = wgpu_surface,
		};
	}

	return EXIT_SUCCESS;
}

void fini_surface(Surface* surface) {
	if (surface == NULL || (*surface) == NULL) return;

	// The wgpu surface can't be deleted so
	// we delete only the windows (only for
	// desktop platforms)
	#if WGPU_DESKTOP
		if ((*surface)->window != NULL) {
			glfwDestroyWindow((*surface)->window);
		}

		glfwTerminate();
	#endif

	// We can free it because we allocate it
	// above...
	free(*surface);
}

#ifdef WGPU_DESKTOP
GLFWwindow* surface_get_glfw_window(Surface* surface) {
	return (*surface)->window;
}
#endif

void* surface_get_raw(Surface* surface) {
	return (void*)((*surface)->surface);
}

void surface_get_size(Surface* surface, uint32_t* width, uint32_t* height) {
	glfwGetFramebufferSize((*surface)->window, (int*)width, (int*)height);
	// glfwGetWindowSize((*surface)->window, (int*)width, (int*)height);
}