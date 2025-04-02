#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 vertNormal;
layout(location = 1) in vec3 lightDir;
layout(location = 2) in vec3 eyeDir; 
layout(location = 3) in vec2 texCoord; 

layout (std140, binding = 1) uniform LightData {
    vec3 position;
    vec4 colour;
};

uniform sampler2D myTexture;

layout(location = 0) out vec4 fragColor;

void main() {
	vec4 kd = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 ks = 0.7 * kd;


	vec4 textureColor = texture(myTexture,texCoord);
	vec4 ka = 0.2 * textureColor;

	float diff = max(dot(vertNormal, lightDir), 0.0);
	/// Reflection is based incedent which means a vector from the light source
	/// not the direction to the light source
	vec3 reflection = normalize(reflect(-lightDir, vertNormal));
	float spec = max(dot(eyeDir, reflection), 0.0);
	spec = pow(spec,14.0);
	vec4 outColor = ka + (textureColor * kd * diff);
	outColor.a = 1.0;
	fragColor =  outColor;	
   
}


//#version 450
//#extension GL_ARB_separate_shader_objects : enable
//
//layout(location = 0) in vec3 vertNormal;
//layout(location = 1) in vec3 lightDir;
//layout(location = 2) in vec3 eyeDir; 
//layout(location = 3) in vec2 texCoord; 
//
//layout (std140, binding = 1) uniform LightData {
//    vec3 position;
//    vec4 colour;
//};
//
//uniform sampler2D myTexture;
//
//layout(location = 0) out vec4 fragColor;
//
//void main() {
//    vec4 baseColor = texture(myTexture, texCoord);
//
//     Diffuse lighting
//    float diff = max(dot(vertNormal, lightDir), 0.0);
//    float intensity = mix(0.3, 1.0, diff);  Softer shading
//
//     Specular highlights (Phong model)
//    vec3 reflectDir = reflect(-lightDir, vertNormal);
//    float spec = pow(max(dot(reflectDir, eyeDir), 0.0), 16.0);  Adjust shininess
//
//     Subtle Rim Lighting
//    float rim = pow(1.0 - max(dot(eyeDir, vertNormal), 0.0), 6.0);
//    vec3 rimLight = vec3(0.2, 0.2, 0.2) * rim;  Less intense
//
//     Final color composition
//    fragColor = vec4(baseColor.rgb * intensity + rimLight + vec3(spec) * 0.2, baseColor.a);
//}
