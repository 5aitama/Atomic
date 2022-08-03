// Atomic.cpp : Defines the entry point for the application.
//

#include <stdio.h>
#include <stdlib.h>

#include "Atomic.h"
#include "surface/surface.h"
#include "renderer/renderer.h"
#include "renderer/material.h"

int main()
{
	Surface s = NULL;
	Renderer r = NULL;
	Material m = NULL;

	// Create a new surface.
	if (init_surface(&s, 800, 600, "Hello world") == EXIT_FAILURE) { return EXIT_FAILURE; }

	// Initialize the renderer.
	if (init_renderer(&r, &s) == EXIT_FAILURE) {
		fini_surface(&s);
		return EXIT_FAILURE; 
	}

	// Initialize a new material.
	if (init_material(&m, &r) == EXIT_FAILURE) {
		fini_renderer(&r);
		fini_surface(&s);
		return EXIT_FAILURE;
	}

#if WGPU_DESKTOP
	// Get the surface window (only for desktop platforms)
	GLFWwindow* window = surface_get_glfw_window(&s);

	// Our render loop...
	while (!glfwWindowShouldClose(window)) {
		if(render_frame(&r) == EXIT_FAILURE) {
			return EXIT_FAILURE;
		}

		glfwPollEvents();
	}
#endif
	
	fini_material(&m);
	fini_renderer(&r);
	fini_surface(&s);

	return EXIT_SUCCESS;
}
