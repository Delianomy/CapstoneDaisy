#version 450
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec4 vVertex;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 uvCoord;


layout (std140, binding = 0) uniform CameraMatricies {
    mat4 projection;
    mat4 view;
};

layout (std140, binding = 1) uniform LightData {
    vec3 position;
    vec4 colour;
};

layout(location = 2) uniform mat4 modelMatrix;


layout(location = 0) out vec3 vertNormal;
layout(location = 1) out vec3 lightDir;
layout(location = 2) out vec3 eyeDir; 
layout(location = 3) out vec2 texCoord; 

uniform vec2 playeranimIndex;
uniform vec2 NPCanimIndex;
uniform int NPC_id;
uniform bool talking;

void main() {
    texCoord = uvCoord;
   
    if(NPC_id == 0){ //player
     texCoord.y *= -1.0;
     texCoord.x = texCoord.x*0.077 + (playeranimIndex.x*0.077);
    }
    if(NPC_id == 1){ //mermaid
        texCoord.y *= -1.0;
          texCoord.y = texCoord.y * 0.5;
            texCoord.x = texCoord.x*0.05882 + (NPCanimIndex.x * 0.05882);
        if(talking){
             texCoord.y *= -1.0;
             texCoord.y = texCoord.y * 0.5 + (NPCanimIndex.y * 0.5);
             texCoord.x = texCoord.x*0.058 + (NPCanimIndex.x *0.058);
        }
    }
  
  
   
    mat3 normalMatrix = mat3(transpose(inverse(view * modelMatrix)));
    vertNormal = normalize(normalMatrix * vNormal); /// Rotate the normal to the correct orientation 
    vec3 vertPos = vec3(view * modelMatrix * vVertex);
    vec3 vertDir = normalize(vertPos);
    eyeDir = -vertDir;
    lightDir = normalize(position - vertPos); 
    gl_Position = projection * view * modelMatrix * vVertex;
}