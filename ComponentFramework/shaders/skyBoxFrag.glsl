#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 fragColor;

uniform samplerCube skybox;

layout(location = 0) in vec3 texCoords;

void main() {
	
	fragColor = texture(skybox, texCoords);
}