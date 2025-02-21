#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 fragColor;
layout(location = 2) uniform vec4 color;

void main() {
    vec4 normalColor = vec4(color.rgb, 0.0);
    fragColor = normalColor;
}