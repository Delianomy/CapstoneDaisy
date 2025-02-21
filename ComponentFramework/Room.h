#pragma once

#include "Actor.h"
#include "ShaderComponent.h"
#include "MeshComponent.h"


class Room : public Actor
{
private:
	GLuint textureID;
	const char* posx, * negx, * posy, * negy, * posz, * negz;
	Ref<MeshComponent> meshComponent;
	Ref<ShaderComponent> shaderComponent;
	float roomSize = 1.0f;
public:

	Room(Component* parent, const char* posx_, const char* negx_, const char* posy_, const char* negy_, const char* posz_, const char* negz_, float scale_);
	~Room();
	bool OnCreate() override;
	void OnDestroy() override;
	void Render();

	GLuint GetTextureID() const { return textureID; }

private:
	bool LoadImages();
};

