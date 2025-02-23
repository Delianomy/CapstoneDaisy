#version 450

#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec4 inVertex;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 uvCoord;

layout(location = 0) out vec2 texCoord; 

layout (std140, binding = 0) uniform CameraMatricies {
    mat4 projection;
    mat4 view;
};


uniform mat4 modelMatrix;
uniform float iTime;

void main() {
    texCoord = uvCoord;
    gl_Position = projection * view * modelMatrix * inVertex;
    
}