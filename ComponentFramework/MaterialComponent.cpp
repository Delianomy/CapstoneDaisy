#define STB_IMAGE_IMPLEMENTATION                                                                                          
#include "MaterialComponent.h"
MaterialComponent::MaterialComponent(Component* parent_,const char* filename_):
	Component(parent_), textureID(0), filename(filename_) {

}



bool MaterialComponent::OnCreate() {
	if (isCreated) return isCreated;
	isCreated  = LoadImage(filename);
	return isCreated;
}

bool MaterialComponent::LoadImage(const char* filename) {

	int channels =0;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	
	unsigned char * textureSurface = stbi_load(filename, &w, &h, &channels, 0);
	if (!textureSurface) {
		glDeleteTextures(1, &textureID);
		return false;
	}




	//int mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureSurface);
	

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if (channels == 3) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, textureSurface);
	}
	else if (channels == 4) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureSurface);
	}
	else {
		// Handle unsupported format
		stbi_image_free(textureSurface);
		glDeleteTextures(1, &textureID);
		return false;
	}

	stbi_image_free(textureSurface);
	glBindTexture(GL_TEXTURE_2D, 0);
	

	/// Wrapping and filtering options

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glBindTexture(GL_TEXTURE_2D, 0); /// Unbind the texture
	return true;
}



MaterialComponent::~MaterialComponent() {
	OnDestroy();
}

void MaterialComponent::OnDestroy() {
	glDeleteTextures(1, &textureID);
}
void MaterialComponent::Update(const float deltaTime) {}
void MaterialComponent::Render()const {}
