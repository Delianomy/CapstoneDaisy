#version 450
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec4 vVertex;



layout(location = 1) uniform mat4 modelMatrix;
layout(location = 2) uniform mat4 viewMatrix;
layout(location = 3) uniform mat4 projectionMatrix;

void main() {
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vVertex;
}