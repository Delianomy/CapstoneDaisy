#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 inVertex;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

layout(location = 0) out vec3 texCoords;

void main() {
    texCoords = inVertex.xyz;
    texCoords.x *= -1;
    gl_Position = projectionMatrix * viewMatrix* modelMatrix * inVertex;
    
}
