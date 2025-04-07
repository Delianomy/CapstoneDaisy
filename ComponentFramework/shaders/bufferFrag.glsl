#version 450
out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D screenTexture;
uniform float time;

void main() {
    // Simulate water distortion using sine wave
//    float wave = sin(20.0 * texCoords.y + time * 2.0) * 0.01;
//    vec2 distortedCoords = texCoords + vec2(wave, 0.0);
//    
    // Add bluish tint
    vec3 color = texture(screenTexture, texCoords).rgb;
//    color.g *= 0.9;
//    color.r *= 0.7;
//    color.b *= 1.2;

    // Optional: apply a slight blur or fog for depth

    FragColor = vec4(color, 1.0);
}
