#include <glew.h>
#include "SkyBox.h"
#include <SDL_image.h>

SkyBox::SkyBox(Component* parent_, const char* posx_, const char* negx_, const char* posy_,
	const char* negy_, const char* posz_, const char* negz_)
	: Actor(parent_), textureID(0) {

	posx = posx_;
	posy = posy_;
	posz = posz_;
	negx = negx_;
	negy = negy_;
	negz = negz_;
}

bool SkyBox::OnCreate() {
	AddComponent<MeshComponent>(nullptr, "meshes/Cube.obj");
	meshComponent = GetComponent<MeshComponent>();
	if (!meshComponent->OnCreate()) { return false; }


	AddComponent<ShaderComponent>(nullptr, "shaders/skyboxVert.glsl", "shaders/skyboxFrag.glsl");
	shaderComponent = GetComponent<ShaderComponent>();
	if (!shaderComponent->OnCreate()) { return false; }

	if (shaderComponent->GetUniformID("modelMatrix") == -1) {
		std::cout << "Failed to find modelMatrix uniform in skybox shader\n";
		return false;
	}


	if (!LoadImages()) return false;
	return true;
}


void SkyBox::OnDestroy() {

	glDeleteTextures(1, &textureID);
}


bool SkyBox::LoadImages() {
	SDL_Surface* textureSurface;
	int mode;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	textureSurface = IMG_Load(posx);
	if (textureSurface == nullptr) {
		return false;
	}
	mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);

	const char* faces[6] = { posx, negx, posy, negy, posz, negz };
	GLenum faceTargets[6] = { GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	  GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	  GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};
	for (int i = 0; i < 6; i++) {
		textureSurface = IMG_Load(faces[i]);
		if (textureSurface == nullptr) {
			return false;
		}
		mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
		glTexImage2D(faceTargets[i], 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
		SDL_FreeSurface(textureSurface);
	}



	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return true;
}

void SkyBox::Render() {
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	//glUseProgram(shaderComponent->GetProgram());
	//glUniformMatrix4fv(shaderComponent->GetUniformID("modelMatrix"), 1, GL_FALSE, GetModelMatrix());
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	meshComponent->Render(GL_TRIANGLES);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);



	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);



}

SkyBox::~SkyBox() {}