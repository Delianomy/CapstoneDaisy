#version 450
out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D normalMap;
uniform float time;



void main() {
    vec2 uv = texCoords.xy;
    vec2 uv1 = uv + time * vec2(0.05, 0.05);
    vec2 uv2 = uv - time * vec2(0.03, 0.04);
    vec3 normal1 = texture(normalMap, uv1).xyz * 2.0 - 1.0;
    vec3 normal2 = texture(normalMap, uv2).xyz * 2.0 - 1.0;
    vec3 normal = normalize(normal1 + normal2);
    
    // Calculate a simple water color based on the normal
    vec3 lightDir = normalize(vec3(0.5, 1.0, 0.3));
    float diffuse = max(dot(normal, lightDir), 0.0);
    vec3 waterColor = vec3(0.0, 0.3, 0.5); // Base blue color
    vec3 finalColor = waterColor * (0.5 + 0.5 * diffuse);
    
    FragColor = vec4(finalColor, 0.9); // Slightly transparent
}
