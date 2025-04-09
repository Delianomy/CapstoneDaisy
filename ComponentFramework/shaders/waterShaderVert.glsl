#version 450
#extension GL_ARB_separate_shader_objects : enable
layout(location = 0) in vec4 vVertex;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 uvCoord;

layout(location = 0) out vec2 fragUV;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec3 fragPos;


uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {

    fragUV = uvCoord;
    fragNormal = mat3(modelMatrix) * vNormal;
    fragPos = vec3(modelMatrix * vVertex);
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vVertex;
}
