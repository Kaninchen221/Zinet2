#version 450

#extension GL_EXT_nonuniform_qualifier : require

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out uint instanceIndex;

layout(set = 1, binding = 0, std140) buffer Models {
	readonly mat4 data[];
} models;

layout(set = 1, binding = 1, std430) buffer Coords {
	readonly float data[];
} coords;

layout(set = 0, binding = 0) uniform Camera {
	mat4 view;
	mat4 projection;
} camera;

void main() {

	mat4 model = models.data[gl_InstanceIndex];
	vec4 position = vec4(inPosition, 1.0);
    gl_Position = camera.projection * camera.view * model * position;
	
	fragColor = inColor;
	
	//uint coordsIndex = (gl_InstanceIndex * 2) + clamp(uint(ceil(gl_VertexIndex / 2)), 0, 0xffffffffU);
	uint coordsIndex = gl_VertexIndex * 2;
	fragTexCoord.r = coords.data[coordsIndex];
	fragTexCoord.g = coords.data[coordsIndex + 1];
	//fragColor = vec4(fragTexCoord.r, fragTexCoord.g, 0, 1);
	
	instanceIndex = gl_InstanceIndex;
}