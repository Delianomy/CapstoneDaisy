#version 450
#extension GL_ARB_separate_shader_objects : enable
layout(location = 0) in vec4 vVertex;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 uvCoord;
uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform float time;
layout(location = 0) out vec2 texCoords;
layout(location = 1) out vec3 worldPos;
layout(location = 2) out vec3 viewDir;
void main() {
    texCoords = uvCoord;
    
    // Generate much more subtle and slower vertex displacement for waves
    // Reduced frequency (0.05 instead of 0.1) and multiplied time by 0.005 to make it much slower
   // Calculate waves in object space before transformation
    float waveHeight = sin(vVertex.x * 0.05 + time * 0.005) * 
                   cos(vVertex.z * 0.05 + time * 0.003) * 0.02;
                   
    // Add height to the vertex in object space
    vec4 displacedVertex = vVertex;
    displacedVertex.y += waveHeight;

    // Then transform to world space after displacement
    vec4 worldPosition = modelMatrix * displacedVertex;
    // Calculate view direction for fresnel effect
    vec3 cameraPos = vec3(inverse(viewMatrix)[3]); // Extract camera position from view matrix
    viewDir = normalize(cameraPos - worldPos);
    
    gl_Position = projectionMatrix * viewMatrix * worldPosition;
}