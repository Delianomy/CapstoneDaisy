#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 texCoord;
layout(location = 0) out vec4 fragColor;
uniform float iTime;

void main() {
    vec2 uv = texCoord;

   // Adjusted values for better visibility
    float amplitude = 0.05;        // Increased amplitude
    float angularVelocity = 5.0;  // Reduced for slower waves
    float frequency = 2.0;        // Reduced for fewer waves
    float offset = 0.5;
    
    float y = amplitude * sin((angularVelocity * uv.x) + (frequency * iTime)) + offset;
    
    vec4 topColor = vec4(0.0, 0.0, 0.0, 1.0);    // Black
    vec4 bottomColor = vec4(0.0, 0.7, 0.9, 1.0);  // Blue
    
    fragColor = uv.y > y ? topColor : bottomColor;
    //fragColor= vec4(1.0, 0.0, 0.0, 0.0);
}