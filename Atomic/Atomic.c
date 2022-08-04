// Atomic.cpp : Defines the entry point for the application.
//

#include <stdio.h>
#include <stdlib.h>

#include "Atomic.h"
#include "surface/surface.h"
#include "context.h"
#include "render_pass.h"
#include "shader.h"
#include "render_pipeline.h"
#include "vertex_buffer.h"

int main()
{
	Surface s = NULL;

	// Create a new surface.
	if (init_surface(&s, 600, 600, "Hello world") == EXIT_FAILURE) 
		return EXIT_FAILURE;
	
	// Initialize Atomic
	atm_init(s);

#if WGPU_DESKTOP
	// Get the surface window (only for desktop platforms)
	GLFWwindow* window = surface_get_glfw_window(&s);

	Shader shader;
	shader_init(&shader, SHADER_FOLDER_PATH "/triangle.wgsl");

	VertexBuffer vbuff = NULL;
	vertex_buffer_init(&vbuff, 3, (float[]) {
			-0.86f, -0.5f, 	1.f, 0.f, 0.f,
			 0.0f,  0.5f, 	0.f, 1.f, 0.f,
			 0.86f, -0.5f,	0.f, 0.f, 1.f,
		},
		(BufferLayout) {
			.layout = (const DataFormat[]) {
				DataFormat_Float32x2,
				DataFormat_Float32x3,
			},
			.count = 2,
		}
	);

	RenderPipeline render_pipeline;
	render_pipeline_init(&render_pipeline, shader, vbuff);

	RenderPass render_pass = NULL;
	render_pass_init(&render_pass);

	// Our render loop...
	while (!glfwWindowShouldClose(window)) {
		atm_begin_render();

		render_pass_begin(render_pass);

		render_pass_set_pipeline(render_pass, render_pipeline);
		render_pass_set_vertex_buffer(render_pass, vbuff);
		render_pass_draw(render_pass, 3, 1, 0, 0);

		render_pass_end(render_pass);

		atm_end_render();
		glfwPollEvents();
	}

	vertex_buffer_fini(&vbuff);
	render_pass_fini(&render_pass);
#endif
	
	render_pipeline_fini(&render_pipeline);
	shader_fini(&shader);

	// Free Atomic
	atm_fini();

	// Free surface
	fini_surface(&s);

	return EXIT_SUCCESS;
}
