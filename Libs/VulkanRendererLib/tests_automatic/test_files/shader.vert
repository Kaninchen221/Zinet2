#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

layout(binding = 0) uniform UniformBufferObject {
    vec2 posOffset;
} ubo;

void main() {
	vec3 position = vec3(inPosition.x + ubo.posOffset.x, inPosition.y + ubo.posOffset.y, inPosition.z);
    gl_Position = vec4(position, 1.0);
    fragColor = inColor;
}