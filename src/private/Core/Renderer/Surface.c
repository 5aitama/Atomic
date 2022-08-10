#include <Atomic/Core/Renderer/Surface.h>
#include "Surface.h"
#include <stdio.h>
#include <stdlib.h>

#if WGPU_DESKTOP
/**
* @brief Initialize a new surface.
*
* @param surface - A pointer to the surface to initialize.
* @param width	 - The window width.
* @param height	 - The window height.
* @param title	 - The window title.
*/
int surface_init(Surface* surface, const uint32_t width, const uint32_t height, const char* title)
#elif WGPU_TARGET == WGPU_TARGET_IOS
/**
* @brief Initialize a new surface.
*
* @param surface - A pointer to the surface to initialize.
* @param mtl_layer - A pointer to the metal layer.
*/
int surface_init(Surface* surface, void* metal_layer)
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

void surface_fini(Surface* surface) {
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

void surface_get_size(Surface* surface, uint32_t* width, uint32_t* height) {
	glfwGetFramebufferSize((*surface)->window, (int*)width, (int*)height);
}

#if WGPU_DESKTOP
int surface_window_should_close(Surface surface) {
	glfwPollEvents();
	return glfwWindowShouldClose(surface->window);
}

float surface_window_get_time() {
	return (float)glfwGetTime();
}
#endif