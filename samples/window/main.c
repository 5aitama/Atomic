#include <stdio.h>
#include <stdlib.h>

#include <Atomic/atomic.h>
#include <Atomic/surface.h>
#include <Atomic/context.h>
#include <Atomic/render_pass.h>
#include <Atomic/shader.h>
#include <Atomic/render_pipeline.h>
#include <Atomic/vertex_buffer.h>
#include <Atomic/uniform.h>
#include <Atomic/texture.h>
#include <Atomic/sampler.h>

int main()
{
	// Create a new surface.
	Surface s = NULL;
	if (init_surface(&s, 600, 600, "Hello world") == EXIT_FAILURE) 
		return EXIT_FAILURE;
	
	// Initialize Atomic
	atm_init(s);

	Texture2D texture2d = NULL;
	texture2D_init(&texture2d, "/Users/alex/Desktop/Atomic/textures/test.jpg");
	
	Sampler sampler = NULL;
	sampler_init(&sampler);

#if WGPU_DESKTOP
	// Get the surface window (only for desktop platforms)
	GLFWwindow* window = surface_get_glfw_window(&s);

	// Create and compile a shader
	Shader shader;
	shader_init(&shader, SHADER_FOLDER_PATH "/triangle.wgsl");

	// Create the buffer that contains the mesh indices.
	Buffer ibuff = NULL;
	buffer_init_with_data(&ibuff, sizeof(uint16_t) * 6, (uint16_t[]){
		0, 1, 2,
		0, 2, 3,
	}, sizeof(uint16_t) * 6, BufferUsage_Index);

	// Create the buffer that contains the mesh vertices.
	VertexBuffer vbuff = NULL;
	const float square_size = 1.f;
	vertex_buffer_init(&vbuff, 4,
		(float[]) {
			-1.0f * square_size, -1.0f * square_size, 	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
			-1.0f * square_size,  1.0f * square_size, 	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
			 1.0f * square_size,  1.0f * square_size,	0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
			 1.0f * square_size, -1.0f * square_size,	1.0f, 1.0f, 1.0f,	1.0f, 0.0f,
		},
		(BufferLayout) {
			.layout = (const DataFormat[]) {
				DataFormat_Float32x2,
				DataFormat_Float32x3,
				DataFormat_Float32x2,
			},
			.count = 3,
		}
	);

	// Create a buffer that contains the uniforms data.
	Buffer uniform_buffer = NULL;
	buffer_init_with_data(&uniform_buffer, sizeof(float) * 4, (float[]){
		1.0f, 0.0f, 0.0f, // The color
		0.0f,			  // The time
	}, sizeof(float) * 4, (BufferUsage)(BufferUsage_Uniform | BufferUsage_CopyDst));

	// Create a group of uniforms data.
	UniformGroup uniform_group = NULL;
	uniform_group_init(&uniform_group, (UniformDescription[]) {
		(UniformDescription) {
			.visibility = UniformVisibility_Fragment,
			.type 		= UniformType_Uniform,
			.size 		= sizeof(float) * 4,
			.offset 	= 0,
			.buffer 	= uniform_buffer,
		},

		(UniformDescription) {
			.visibility = UniformVisibility_Fragment,
			.type		= UniformType_Uniform,
			.sampler	= sampler,
		},

		(UniformDescription) {
			.visibility = UniformVisibility_Fragment,
			.type		= UniformType_Uniform,
			.texture 	= texture2d,
		},
	}, 3);

	// Create a render pipeline.
	RenderPipeline render_pipeline;
	render_pipeline_init(&render_pipeline, shader, vbuff, &uniform_group, 1);

	// Create a render pass.
	RenderPass render_pass = NULL;
	render_pass_init(&render_pass);

	// Our render loop...
	while (!glfwWindowShouldClose(window)) {
		float curr_time = (float)glfwGetTime();
		atm_begin_render();

		buffer_write(uniform_buffer, sizeof(float) * 3, (float[]){ curr_time * 2 }, sizeof(float));
		render_pass_begin(render_pass);
		
		render_pass_set_pipeline(render_pass, render_pipeline);
		render_pass_set_uniform_groups(render_pass, &uniform_group, 1);
		render_pass_set_vertex_buffer(render_pass, vbuff);
		render_pass_set_index_buffer(render_pass, ibuff, IndexFormat_uint16, 0, sizeof(uint16_t) * 6);
		render_pass_draw_indexed(render_pass, 6, 1, 0, 0, 0);

		render_pass_end(render_pass);

		atm_end_render();
		glfwPollEvents();
	}

	sampler_fini(&sampler);
	texture2D_fini(&texture2d);
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
