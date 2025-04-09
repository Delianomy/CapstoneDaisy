#version 450
layout(location = 0) in vec2 fragUV;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec3 fragPos;
layout(location = 0) out vec4 outColor;
uniform sampler2D waterNormalMap;
uniform vec3 cameraPos;
uniform float time;
void main() {
    // Mix world position with time-based animation
    vec2 worldBasedUV = fragPos.xz * 0.1;
    
    // Use fixed world-space animation vectors that don't depend on camera movement
    vec2 flowDirection1 = vec2(1.0, 0.0);
    vec2 flowDirection2 = vec2(0.0, 1.0);
    
    // Create two layers of animation moving in different directions
    vec2 animatedUV1 = worldBasedUV + flowDirection1 * time * 0.05;
    vec2 animatedUV2 = worldBasedUV + flowDirection2 * time * 0.03;
    
    // Sample normal map twice and blend
    vec3 normalMap1 = texture(waterNormalMap, animatedUV1).rgb * 2.0 - 1.0;
    vec3 normalMap2 = texture(waterNormalMap, animatedUV2).rgb * 2.0 - 1.0;
    vec3 blendedNormal = normalize(normalMap1 + normalMap2);
    
    vec3 normal = normalize(mix(fragNormal, blendedNormal, 0.7));
    
    // Lighting
    vec3 lightDir = normalize(vec3(0.3, 1.0, 0.4));
    float diff = max(dot(normal, lightDir), 0.0);
    
    // Specular highlight
    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    
    vec3 waterColor = vec3(0.1, 0.4, 0.7);
    vec3 color = waterColor * diff + vec3(0.5) * spec;
    outColor = vec4(color, 0.8); // Added some transparency
}