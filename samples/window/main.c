#include <stdio.h>
#include <stdlib.h>

#include <Atomic/Core/Renderer/Surface.h>
#include <Atomic/Core/Renderer/Context.h>
#include <Atomic/Core/Renderer/PassEncoder.h>
#include <Atomic/Core/Renderer/RenderPass.h>

void my_pass(RenderPass pass, void* user_data) {
	render_pass_add_color_attachment(pass, 217.0 / 255.0, 217.0 / 255.0,  217.0 / 255.0, 1.0);
	render_pass_begin(pass);
	render_pass_end(pass);
}

int main()
{	
	// Create a new surface. Notice that this
	// code only works on desktop platforms (
	// Linux, Windows, macOS) for mobile you
	// must pass a pointer to the native
	// surface.
	Surface surface = NULL;
	surface_init(&surface, 800, 600, "Atomic Sample");

	// Initialize the renderer. This tell the
	// renderer that all drawing operations
	// will be on the surface created above.
	renderer_init(surface);

	float last_time = surface_window_get_time();
	int fps = 0;
	float acc = 0;

	while(!surface_window_should_close(surface)) {
		float curr_time = surface_window_get_time();
		float delta_time = curr_time - last_time;
		last_time = curr_time;

		if (acc >= 1) {
			printf("%i FPS\n", fps);
			acc = 0;
			fps = 0;
		}

		acc += delta_time;
		fps ++;

		// This will create a texture on wich
		// the renderer can draw. This must be
		// called at the begining of each frame
		// or before any rendering operations.
		renderer_aquire_texture();

		// Create a new pass encoder. We will
		// record all things that we want to
		// do on the gpu (like rendering and
		// compute) in a PassEncoder.
		PassEncoder encoder = NULL;
		pass_encoder_init(&encoder);

		// Encode our render pass into the
		// pass encoder created above.
		pass_encoder_encode_passes(encoder, (PassDescriptor[]) {
			(PassDescriptor) {
				.type = PassType_RenderPass,
				.func = (PassFunc)my_pass,
			},
		}, 1, NULL);

		// Submit the pass encoder to the
		// gpu for execution. Notice that
		// we can submit multiple pass 
		// encoder per frame.
		pass_encoder_submit(&encoder, 1);
		pass_encoder_fini(&encoder);

		// Present the current texture
		// into the surface.
		renderer_present();
	}

	// Release created structs.
	renderer_fini();
	surface_fini(&surface);

	return EXIT_SUCCESS;
}
