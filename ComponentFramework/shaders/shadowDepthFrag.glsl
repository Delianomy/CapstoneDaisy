#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out float fragDepth;

void main() {
    /// This is not needed, the driver does it automatically
    fragDepth = gl_FragCoord.z;
}