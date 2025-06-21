#version 450

#extension GL_EXT_nonuniform_qualifier : require

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out uint instanceIndex;

layout(set = 1, binding = 0) uniform Uniforms {
    vec2 posOffset;
	float colorScalar;
} uniforms[];

void main() {

	instanceIndex = gl_InstanceIndex;

	vec2 posOffset = uniforms[gl_InstanceIndex].posOffset;
	float colorScalar = uniforms[0].colorScalar;

	float instanceYOffset = gl_InstanceIndex * 0.5;
	vec3 position = vec3(inPosition.x + posOffset.x, inPosition.y + posOffset.y + instanceYOffset, inPosition.z);
    gl_Position = vec4(position, 1.0);
    fragColor = inColor;
	fragColor *= colorScalar;
    fragTexCoord = inTexCoord;
}