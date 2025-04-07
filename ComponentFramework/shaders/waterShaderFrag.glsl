#version 450
out vec4 FragColor;
in vec2 texCoords;
in vec3 worldPos;
in vec3 viewDir;
uniform sampler2D normalMap;
uniform float time;
void main() {
    // Create slower UV animations with reduced speed factors
    vec2 uv = texCoords.xy;
    
    // First normal layer - larger waves - reduced speed by factor of 10
    vec2 uv1 = uv + time * vec2(0.003, 0.005);
    vec3 normal1 = texture(normalMap, uv1).xyz * 2.0 - 1.0;
    
    // Second normal layer - smaller waves - reduced speed
    vec2 uv2 = uv - time * vec2(0.004, 0.003);
    vec3 normal2 = texture(normalMap, uv2 * 1.2).xyz * 2.0 - 1.0; // Reduced scaling from 1.5 to 1.2
    
    // Third normal layer - smallest detail waves - reduced speed and impact
    vec2 uv3 = uv + time * vec2(-0.002, 0.006);
    vec3 normal3 = texture(normalMap, uv3 * 1.8).xyz * 2.0 - 1.0; // Reduced scaling from 2.5 to 1.8
    
    // Combine normals with different weights - increased weight of main waves, reduced detail
    vec3 normal = normalize(normal1 * 0.7 + normal2 * 0.2 + normal3 * 0.1);
    
    // Lighting calculation
    vec3 lightDir = normalize(vec3(0.5, 1.0, 0.3));
    float diffuse = max(dot(normal, lightDir), 0.0);
    
    // Significantly reduced specular highlight
    vec3 reflectDir = reflect(-lightDir, normal);
    float specular = pow(max(dot(viewDir, reflectDir), 0.0), 16.0) * 0.15; // Reduced exponent and multiplier
    
    // Reduced fresnel effect
    float fresnel = pow(1.0 - max(dot(normal, viewDir), 0.0), 3.0) * 0.5; // Reduced exponent and added multiplier
    
    // Deeper, more muted water colors
    vec3 deepColor = vec3(0.0, 0.08, 0.15); // Darker deep color
    vec3 shallowColor = vec3(0.0, 0.3, 0.5); // Less saturated shallow color
    
    // Mix colors based on normal direction (simulating depth)
    float waterDepth = normal.y * 0.5 + 0.5;
    vec3 waterColor = mix(deepColor, shallowColor, waterDepth);
    
    // Add lighting effects with reduced specular
    vec3 finalColor = waterColor * (0.7 + 0.3 * diffuse) + vec3(0.8, 0.9, 1.0) * specular;
    
    // Much subtler rim lighting
    finalColor = mix(finalColor, vec3(0.1, 0.4, 0.5), fresnel * 0.2);
    
    // Slightly more consistent transparency
    float alpha = mix(0.75, 0.82, fresnel);
    
    FragColor = vec4(finalColor, alpha);
}