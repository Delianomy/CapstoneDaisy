#include "LightActor.h"
#include "CameraActor.h"
#include "UBO_Padding.h"
#include "MMath.h"
using namespace MATH;

LightActor::LightActor(Component* parent_, LightStyle lightStyle_, Vec3 location_, Vec4  colour_,
	float intensity, Vec3 fallOff ): Actor(parent_) {
	lightStyle = lightStyle_;
	position = location_;
	colour = colour_;
	

}
void LightActor::UpdateUBO() const {
	Matrix4 viewMatrix; /// default identity matrix
	
	glBindBuffer(GL_UNIFORM_BUFFER, uboLightDataID);
	size_t buffer_size = UBO_PADDING::VEC3 + UBO_PADDING::VEC4;
	size_t offset = 0;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(Vec3),position);
	offset = UBO_PADDING::VEC3;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(Vec4), colour);
	
}


bool LightActor::OnCreate() {
	if (isCreated) return isCreated;

	depthProjectionMatrix = MMath::orthographic(-7, 7, -10, 10, -10, 20);
	depthViewMatrix = MMath::lookAt(-position, Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));

	size_t buffer_size = UBO_PADDING::VEC3 + UBO_PADDING::VEC4;
	glGenBuffers(1, &uboLightDataID);
	glBindBuffer(GL_UNIFORM_BUFFER, uboLightDataID);
	glBufferData(GL_UNIFORM_BUFFER, buffer_size, nullptr, GL_DYNAMIC_DRAW);
	UpdateUBO();
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	/// To connect this ubo to the bindingpoint, the buffer does not need to be see the above line 
	glBindBufferBase(GL_UNIFORM_BUFFER, bindPoint, uboLightDataID);



	glGenFramebuffers(1, &shadowBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowBufferID);
	
	glGenTextures(1, &depthTextureID);
	glBindTexture(GL_TEXTURE_2D, depthTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, 1024, 1024, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTextureID, 0);
	glDrawBuffer(GL_NONE); 
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	isCreated = true;
	return isCreated;
}

LightActor::~LightActor() {
	OnDestroy();
}


void LightActor::Render() const {


}

void LightActor::OnDestroy() {
	glDeleteBuffers(1, &uboLightDataID);
	glDeleteTextures(1, &depthTextureID);
	glDeleteFramebuffers(1, &shadowBufferID);
	isCreated = false;
}
