#version 450

#extension GL_EXT_nonuniform_qualifier : require

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) flat in uint instanceIndex;

layout(location = 0) out vec4 outColor;

layout(set = 1, binding = 1) uniform sampler2D texSamplers[];

void main() {
    outColor = fragColor;
	
	vec4 texColor = texture(texSamplers[instanceIndex], fragTexCoord);
	outColor.rgba *= texColor.rgba;
}