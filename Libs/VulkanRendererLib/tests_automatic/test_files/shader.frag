#version 450

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D texSampler;

void main() {
    outColor = fragColor;
	vec4 texColor = texture(texSampler, fragTexCoord);
	outColor.rgb *= texColor.rgb;
}