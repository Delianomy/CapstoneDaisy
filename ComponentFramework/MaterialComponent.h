
#pragma once


#include "stb_image.h"
#include <glew.h>
#include "Component.h"



class MaterialComponent: public Component {
	/// Unless you know what these do don't allow them
	/// to be created implicitly 
	MaterialComponent(const MaterialComponent&) = delete;
	MaterialComponent(MaterialComponent&&) = delete;
	MaterialComponent& operator=(const MaterialComponent&) = delete;
	MaterialComponent& operator=(MaterialComponent&&) = delete;
private:
	GLuint textureID;
	const char* filename;
	bool LoadImage(const char* filename);
public:
	MaterialComponent(Component* parent_,const char* filename_);
	virtual ~MaterialComponent();
	
	inline GLuint getTextureID() const { return textureID; }

	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime_);
	virtual void Render()const;
};

