// Atomic.cpp : Defines the entry point for the application.
//

#include <stdio.h>
#include <stdlib.h>

#include "Atomic.h"
#include "surface/surface.h"

int main()
{
	Surface s = NULL;

	// Create a new surface.
	if (init_surface(&s, 800, 600, "Hello world") == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

#if WGPU_DESKTOP
	// Get the surface window (only for desktop platforms)
	GLFWwindow* window = surface_get_glfw_window(&s);

	// Our render loop...
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
#endif
	
	// Destroy the surface properly.
	fini_surface(&s);

	return EXIT_SUCCESS;
}
