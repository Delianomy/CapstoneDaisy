#pragma once
#include <glew.h>
#include "Actor.h"
#include "Vector.h"
using namespace MATH;

enum class LightStyle {
	DirectionLight = 1,
	PointLight,
	SkyLight,
	SpotLight
};

class LightActor : public Actor {
	LightActor(const LightActor&) = delete;
	LightActor(LightActor&&) = delete;
	LightActor& operator=(const LightActor&) = delete;
	LightActor& operator=(LightActor&&) = delete;
private:
	LightStyle lightStyle;
	Vec3 position;
	Vec4 colour;
	float intensity;
	Vec3 fallOff;
	GLuint shadowBufferID;
	GLuint depthTextureID;

	GLuint uboLightDataID;
	const GLuint bindPoint = 1;
public:
	LightActor(Component* parent_, LightStyle lightStyle_, Vec3 location_, Vec4  colour_,
		float intensity = 1.0f, Vec3 fallOff = Vec3(0.0f, 0.0f, 0.0f));
	~LightActor();
	Vec3 GetPosition() const { return position; }
	void SetPosition(const Vec3 position_ ) { 
		position = position_; 
		UpdateUBO();
	}
	void UpdateUBO() const;
	bool OnCreate() override;
	void OnDestroy() override;
	void Render() const override;
	GLuint GetLightID() { return uboLightDataID; }

	/// just for debugging
	Matrix4 depthProjectionMatrix;
	Matrix4 depthViewMatrix;

};

