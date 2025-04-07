#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec2 texCoords;

void main() {
    vec2 positions[3] = vec2[](
        vec2(-1.0, -1.0),
        vec2( 3.0, -1.0),
        vec2(-1.0,  3.0)
    );

    gl_Position = vec4(positions[gl_VertexID], 0.0, 1.0);

    // Convert from [-1,1] range to [0,1] texture coordinates
    texCoords = gl_Position.xy * 0.5 + 0.5;
}