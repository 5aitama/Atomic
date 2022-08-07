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
#include "buffer/uniform/uniform.h"

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

	Buffer ibuff = NULL;
	buffer_init_with_data(&ibuff, sizeof(uint16_t) * 6, (uint16_t[]){
		0, 1, 2,
		0, 2, 3,
	}, sizeof(uint16_t) * 6, BufferUsage_Index);

	VertexBuffer vbuff = NULL;
	vertex_buffer_init(&vbuff, 4, (float[]) {
			-1.0f, -1.0f, 	1.0f, 0.0f, 0.0f,
			-1.0f,  1.0f, 	0.0f, 1.0f, 0.0f,
			 1.0f,  1.0f,	0.0f, 0.0f, 1.0f,
			 1.0f, -1.0f,	1.0f, 1.0f, 1.0f,
		},
		(BufferLayout) {
			.layout = (const DataFormat[]) {
				DataFormat_Float32x2,
				DataFormat_Float32x3,
			},
			.count = 2,
		}
	);

	Buffer uniform_buffer = NULL;
	buffer_init_with_data(&uniform_buffer, sizeof(float) * 4, (float[]){
		1.0f, 0.0f, 0.0f, // The color
		0.0f,			  // The time
	}, sizeof(float) * 4, (BufferUsage)(BufferUsage_Uniform | BufferUsage_CopyDst));

	UniformGroup uniform_group = NULL;
	uniform_group_init(&uniform_group, &(UniformDescription){
		.visibility = UniformVisibility_Fragment,
		.type 		= UniformType_Uniform,
		.size 		= sizeof(float) * 4,
		.offset 	= 0,
		.buffer 	= uniform_buffer,
	}, 1);

	RenderPipeline render_pipeline;
	render_pipeline_init(&render_pipeline, shader, vbuff, &uniform_group, 1);

	RenderPass render_pass = NULL;
	render_pass_init(&render_pass);

	// Our render loop...
	while (!glfwWindowShouldClose(window)) {
		float curr_time = (float)glfwGetTime();
		atm_begin_render();

		render_pass_begin(render_pass);

		buffer_write(uniform_buffer, sizeof(float) * 3, (float[]){ curr_time * 5 }, sizeof(float));
		render_pass_set_pipeline(render_pass, render_pipeline);
		render_pass_set_uniform_groups(render_pass, &uniform_group, 1);
		render_pass_set_vertex_buffer(render_pass, vbuff);
		render_pass_set_index_buffer(render_pass, ibuff, IndexFormat_uint16, 0, sizeof(uint16_t) * 6);
		render_pass_draw_indexed(render_pass, 6, 1, 0, 0, 0);

		render_pass_end(render_pass);

		atm_end_render();
		glfwPollEvents();
	}

	vertex_buffer_fini(&vbuff);
	render_pass_fini(&render_pass);
#endif
	uniform_group_fini(&uniform_group);
	buffer_fini(&uniform_buffer);
	buffer_fini(&ibuff);
	render_pipeline_fini(&render_pipeline);
	shader_fini(&shader);

	// Free Atomic
	atm_fini();

	// Free surface
	fini_surface(&s);

	return EXIT_SUCCESS;
}
