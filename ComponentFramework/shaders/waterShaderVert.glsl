#version 450
#extension GL_ARB_separate_shader_objects : enable
layout(location = 0) in vec4 vVertex;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 uvCoord;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;


layout(location = 0) out vec2 texCoords;



void main() {
    texCoords = uvCoord;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vVertex;
}