#version 450

#extension GL_EXT_nonuniform_qualifier : require

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out uint instanceIndex;

layout(set = 1, binding = 0) uniform Uniforms {
	mat4 model;
	mat4 view;
	mat4 projection;
} uniforms[];

layout(set = 0, binding = 0) uniform Camera {
	mat4 view;
	mat4 projection;
} camera;

void main() {
	mat4 model = uniforms[gl_InstanceIndex].model;
	mat4 view = camera.view;
	mat4 projection = camera.projection;

	vec4 position = vec4(inPosition, 1.0);
    gl_Position = projection * view * model * position;
    fragColor = inColor;
    fragTexCoord = inTexCoord;
	instanceIndex = gl_InstanceIndex;
}