#version 450

#extension GL_EXT_nonuniform_qualifier : require

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) flat in uint instanceIndex;

layout(location = 0) out vec4 outColor;

//layout(set = 0, binding = 0) uniform sampler2D texSamplers[];

void main() {
    outColor = fragColor;
	
	//vec4 texColor = texture(texSamplers[0], fragTexCoord); // "0" For now ignore texture indexing
	//outColor.rgb *= texColor.rgb;
}