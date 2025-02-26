struct VertexInput {
    @location(0) position: vec3f,
    @location(1) normal: vec3f,
    @location(2) color: vec3f,
    @location(3) uv: vec2f,
    @location(4) tangent: vec3f,
    @location(5) bitangent: vec3f,
};

struct VertexOutput {
    @builtin(position) position: vec4f,
    @location(0) color: vec3f,
    @location(1) normal: vec3f,
    @location(2) uv: vec2f,
    @location(3) viewDirection: vec3f,
    @location(4) tangent: vec3f,
    @location(5) bitangent: vec3f
}

struct MyUniforms {
    projectionMatrix: mat4x4f,
    viewMatrix: mat4x4f,
    color: vec4f,
    cameraWorldPosition: vec3f,
    time: f32,
};

struct PerObjectUniforms {
    modelMatrix: mat4x4f,
}

@group(0) @binding(0) var<uniform> uPerObjectUniforms: PerObjectUniforms;
@group(1) @binding(0) var<uniform> uMyUniforms: MyUniforms;

@vertex
fn vs_main(in: VertexInput) -> VertexOutput {
	var out: VertexOutput;
    
    let worldPosition = uPerObjectUniforms.modelMatrix * vec4f(in.position, 1.0);
    out.position = uMyUniforms.projectionMatrix * uMyUniforms.viewMatrix * worldPosition;

	let cameraWorldPosition = uMyUniforms.cameraWorldPosition;
    out.viewDirection = cameraWorldPosition - worldPosition.xyz;

    out.tangent = (uPerObjectUniforms.modelMatrix * vec4f(in.tangent, 0.0)).xyz;
    out.bitangent = (uPerObjectUniforms.modelMatrix * vec4f(in.bitangent, 0.0)).xyz;
    out.normal = (uPerObjectUniforms.modelMatrix * vec4f(in.normal, 0.0)).xyz;

	out.color = in.color;
    out.uv = in.uv;
    return out;
}

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4f {
    let localToWorld = mat3x3f(
        normalize(in.tangent),
        normalize(in.bitangent),
        normalize(in.normal),
    );
    
    let color = in.color;

    // Gamma-correction
    let corrected_color = pow(color.xyz, vec3f(2.2));
    return vec4f(corrected_color, 1.0);
}