#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

layout(binding = 0) uniform Uniforms {
    vec2 posOffset;
	float colorScalar;
} uniforms;

void main() {
	vec3 position = vec3(inPosition.x + uniforms.posOffset.x, inPosition.y + uniforms.posOffset.y, inPosition.z);
    gl_Position = vec4(position, 1.0);
    fragColor = inColor;
	fragColor *= uniforms.colorScalar;
}