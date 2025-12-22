#version 450

#extension GL_EXT_nonuniform_qualifier : require

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out uint instanceIndex;

layout(set = 1, binding = 0) buffer Positions {
	readonly vec3 value;
} positions[];

layout(set = 1, binding = 1) buffer Rotations {
	readonly float value;
} rotations[];

layout(set = 1, binding = 2) buffer Scales {
	readonly vec3 value;
} scales[];


layout(set = 0, binding = 0) uniform Camera {
	mat4 view;
	mat4 projection;
} camera;

void main() {
	// TODO: Create a model matrix from Positions, Rotations and Scales
	//mat4 model = instances.model[gl_InstanceIndex];
	//vec3 position = instances[gl_InstanceIndex].position;

	//vec4 position = vec4(inPosition, 1.0);
    //gl_Position = camera.projection * camera.view * model * position;
	fragColor = inColor;
    fragTexCoord = inTexCoord;
	instanceIndex = gl_InstanceIndex;
}