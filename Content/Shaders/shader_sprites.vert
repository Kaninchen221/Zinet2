#version 450

#extension GL_EXT_nonuniform_qualifier : require

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out uint instanceIndex;

layout(set = 1, binding = 0) buffer Transforms {
	mat4 model;
} transforms[];

void main() {
	mat4 model = transforms[gl_InstanceIndex].model;
	//mat4 view = uniforms[gl_InstanceIndex].view; // TODO
	//mat4 projection = uniforms[gl_InstanceIndex].projection; // TODO

	vec4 position = vec4(inPosition, 1.0);
	gl_Position = position;
    //gl_Position = projection * view * model * position;
    fragColor = inColor;
    fragTexCoord = inTexCoord;
	instanceIndex = gl_InstanceIndex;
}