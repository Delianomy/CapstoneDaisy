#pragma once
#include "Actor.h"
#include "Matrix.h"
using namespace MATH;

class CameraActor:public Actor {
	/// Unless you know what these do don't allow them
	/// to be created implicitly 
	CameraActor(const CameraActor&) = delete;
	CameraActor(CameraActor&&) = delete;
	CameraActor& operator=(const CameraActor&) = delete;
	CameraActor& operator=(CameraActor&&) = delete;

private:
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
	/// This is experimental for an Umer project: ray casting and ray tracing 
	Matrix4 rayCastMatrix;

	unsigned int uboMatriciesID;
	const GLuint bindingPoint; /// This must be unique to the camera = 0, lights = 1

public:
	bool isInMainMenu = false;
	Matrix4 orient;
	CameraActor(Component* parent_);
	virtual ~CameraActor();

	bool OnCreate() override;
	void OnDestroy() override;
	Matrix4 GetProjectionMatrix() const { return projectionMatrix; }
	Matrix4 GetViewMatrix() const { return viewMatrix; }

	void UpdateProjectionMatrix(const float fovy, const float aspectRatio, const float near, const float far);
	void UpdateViewMatrix();
	void UpdateRayCastMatrix();
	GLuint GetMatriciesID() const { return uboMatriciesID; }
};

