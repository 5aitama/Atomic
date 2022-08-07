struct VertexInput {
    @location(0) position: vec2<f32>,
    @location(1) color: vec3<f32>,
};

struct VertexOutput {
    @builtin(position) clip_position: vec4<f32>,
    @location(0) color: vec3<f32>,
};

struct BackgroundColor {
    value: vec3<f32>,
    time: f32,
};

@group(0) @binding(0)
var<uniform> background_color: BackgroundColor;

@vertex
fn vs_main(
    model: VertexInput,
) -> VertexOutput {
    var out: VertexOutput;
    out.color = model.color;
    out.clip_position = vec4<f32>(model.position, 0.0, 1.0);
    return out;
}

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4<f32> {
    var s : f32 = (sin(background_color.time) + 1.0) / 2.0;
    var c : f32 = (cos(background_color.time) + 1.0) / 2.0;
    return vec4<f32>(s, c, s, 1.0);
}