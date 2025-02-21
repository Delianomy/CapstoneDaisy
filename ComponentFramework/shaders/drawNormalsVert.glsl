#version 450
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec4 inVertex;
layout(location = 1) in vec3 inNormal;

layout (std140, binding = 0) uniform CameraMatricies {
    mat4 projection;
    mat4 view;
};

out VertexStage {
    vec3 normal;
} vs_out;

layout(location = 1) uniform mat4 modelMatrix;


void main() {
    gl_Position = view * modelMatrix * inVertex;
    mat3 normalMatrix = mat3(transpose(inverse(view * modelMatrix)));
    vs_out.normal = normalMatrix * inNormal;
}