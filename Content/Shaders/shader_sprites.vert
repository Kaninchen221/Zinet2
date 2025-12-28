#version 450

#extension GL_EXT_nonuniform_qualifier : require

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out uint instanceIndex;

layout(set = 1, binding = 0) buffer Positions {
	readonly vec4 values[];
} position;

// Radians
layout(set = 1, binding = 1) buffer Rotations {
	readonly float values[];
} rotation;

layout(set = 1, binding = 2) buffer Scales {
	readonly vec4 values[];
} scale;


layout(set = 0, binding = 0) uniform Camera {
	mat4 view;
	mat4 projection;
} camera;

void main() {

	vec4 position = position.values[gl_InstanceIndex];
	vec4 scale = scale.values[gl_InstanceIndex];
	float rotation = rotation.values[gl_InstanceIndex];

	float rotation_c = cos(rotation);
	float rotation_s = sin(rotation);

	mat4 model = mat4(
		scale.x * rotation_c, -scale.y * rotation_s, 0.0, 0.0,
		scale.x * rotation_s,  scale.y * rotation_c, 0.0, 0.0,
		0.0, 0.0, scale.z, 0.0,
		position.x, position.y, position.z, 1.0
	);
	
    gl_Position = camera.projection * camera.view * model * vec4(inPosition, 1.0);
	fragColor = inColor;
    fragTexCoord = inTexCoord;
	instanceIndex = gl_InstanceIndex;
}