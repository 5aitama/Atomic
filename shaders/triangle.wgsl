struct VertexInput {
    @location(0) position: vec2<f32>,
    @location(1) color: vec3<f32>,
    @location(2) uv: vec2<f32>,
};

struct VertexOutput {
    @builtin(position) clip_position: vec4<f32>,
    @location(0) color: vec3<f32>,
    @location(1) uv: vec2<f32>,
};

struct BackgroundColor {
    value: vec3<f32>,
    time: f32,
};

@group(0) @binding(0)
var<uniform> background_color: BackgroundColor;

@group(0) @binding(1)
var t_sampler: sampler;

@group(0)@binding(2)
var t_texture: texture_2d<f32>;

@vertex
fn vs_main(model: VertexInput) -> VertexOutput {
    var out: VertexOutput;
    out.color = model.color;
    out.clip_position = vec4<f32>(model.position, 0.0, 1.0);
    out.uv = model.uv;
    return out;
}

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4<f32> {
    var s : f32 = (sin(background_color.time) + 1.0) / 2.0;
    var c : f32 = (cos(background_color.time) + 1.0) / 2.0;

    let r : f32 = 1920.0 / 1080.0;
    let m = vec2<f32>(1. / r, 1.);
    let offset = vec2<f32>(-0.22, 0.0);
    return textureSample(t_texture, t_sampler, vec2<f32>(0.0, 1.0) - (in.uv * m) + offset);
}