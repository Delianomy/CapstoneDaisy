#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 vVertex;
layout(location = 0) uniform mat4 depthProjectionMatrix;
layout(location = 1) uniform mat4 depthViewMatrix;
layout(location = 2) uniform mat4 modelMatrix;


void main() {
    gl_Position = depthProjectionMatrix * depthViewMatrix * modelMatrix * vVertex;
}