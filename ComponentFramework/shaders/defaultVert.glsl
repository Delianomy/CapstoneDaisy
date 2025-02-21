#version 450

#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec4 inVertex;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 uvCoord;

layout(location = 3) out vec2 texCoord; 

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;


void main() {
    texCoord = uvCoord;
    texCoord.y *= -1.0;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * inVertex;
    
}