#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (triangles) in; /// bringing in triangles
layout (line_strip, max_vertices = 2) out; /// output is lines w/ 6 verticies

layout (std140, binding = 0) uniform CameraMatricies {
    mat4 projection;
    mat4 view;
};

layout(location = 1) uniform mat4 modelMatrix;
layout(location = 2) uniform vec4 color;

in VertexStage {
    vec3 normal;
} gs_in[];



void main() {
    float length = color.w; /// use w to hold the length of the normal 
    for(int index = 0; index < 3; index++){
        gl_Position = projection * gl_in[index].gl_Position;
        EmitVertex();

        gl_Position = projection * (gl_in[index].gl_Position + vec4(gs_in[index].normal, 1.0) * length);
        EmitVertex();
        EndPrimitive();
    }
}