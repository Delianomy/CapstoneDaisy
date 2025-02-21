#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Debug.h"
#include "ShaderTestScene.h"
#include "MMath.h"
#include "Debug.h"
#include "CameraActor.h"
#include "LightActor.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "MaterialComponent.h"


#include <string>
using namespace MATH;


ShaderTestScene::ShaderTestScene() :drawNormals(false), drawOverlay(false) {
	Debug::Info("Created Scene SHADER TEST: ", __FILE__, __LINE__);
	glGetIntegerv(GL_VIEWPORT, viewport);
}

bool ShaderTestScene::OnCreate() {
	Debug::Info("Loading assets Scene SHADER TEST: ", __FILE__, __LINE__);
	assetManager = std::make_shared<AssetManager>();

	Ref<ShaderComponent> shader = assetManager->GetComponent<ShaderComponent>("TextureShader");
	Ref<ShaderComponent> CubeShader = assetManager->GetComponent<ShaderComponent>("RefularTexture");
	Ref<ShaderComponent> WaveShader = assetManager->GetComponent<ShaderComponent>("WaveShader");



	camera = std::make_shared<CameraActor>(nullptr);
	camera->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, -4.0f), Quaternion());
	camera->OnCreate();



	light = std::make_shared<LightActor>(camera.get(), LightStyle::DirectionLight, Vec3(0.0f, 5.0f, 1.0f), Vec4(0.85f, 0.6, 0.6f, 0.0f));
	light->OnCreate();

	cube = std::make_shared<Actor>(nullptr);
	//cube->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, -3.0f), orientation);
	//cube->GetComponent<TransformComponent>()->SetScale(Vec3(0.1f, 0.1f, 0.1f));
	cube->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, 0.0f),/// pos
		QMath::angleAxisRotation(0.0f, Vec3(1.0f, 0.0f, 0.0f)), Vec3(0.0f, 0.0f, 0.0f));
	cube->GetComponent<TransformComponent>()->SetScale(Vec3(1.0f, 1.0f, 1.0f));

	cube->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Square"));
	cube->AddComponent<ShaderComponent>(CubeShader);
	cube->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("ChessBoard"));
	cube->OnCreate();



	
	

	return true;
}





ShaderTestScene::~ShaderTestScene() {
	Debug::Info("Deleted Scene Dream: ", __FILE__, __LINE__);
	OnDestroy();
}

void ShaderTestScene::OnDestroy() {
	Debug::Info("Deleting Scene Dream: ", __FILE__, __LINE__);
}

void ShaderTestScene::HandleEvents(const SDL_Event& sdlEvent) {
	static int objID = -1;
	static Vec2 currentMousePos;
	static Vec2	lastMousePos;
	static float flip = 1.0f;
	Ref<TransformComponent> cameraTC;
	Ref<TransformComponent> gameBoardTC;

	/// Handle Camera movement 
	switch (sdlEvent.type) {
	case SDL_KEYDOWN:
		cameraTC = camera->GetComponent<TransformComponent>();
		switch (sdlEvent.key.keysym.scancode) {


		case SDL_SCANCODE_LEFT:
		{
			Quaternion rotate = cameraTC->GetQuaternion() * QMath::angleAxisRotation(-1.0f, Vec3(0.0f, 1.0f, 0.0f));
			cameraTC->SetTransform(cameraTC->GetPosition(), rotate);
			camera->UpdateViewMatrix();
			light->UpdateUBO();
			break;
		}

		case  SDL_SCANCODE_RIGHT:
		{
			Quaternion rotate = cameraTC->GetQuaternion() * QMath::angleAxisRotation(1.0f, Vec3(0.0f, 1.0f, 0.0f));
			cameraTC->SetTransform(cameraTC->GetPosition(), rotate);
			camera->UpdateViewMatrix();
			light->UpdateUBO();
			break;
		}

		case SDL_SCANCODE_UP:
			cameraTC->SetTransform(cameraTC->GetPosition() + Vec3(0.0f, 0.0f, 0.1f), cameraTC->GetQuaternion());
			camera->UpdateViewMatrix();
			break;

		case SDL_SCANCODE_DOWN:
			cameraTC->SetTransform(cameraTC->GetPosition() + Vec3(0.0f, 0.0f, -0.1f), cameraTC->GetQuaternion());
			camera->UpdateViewMatrix();
			break;

		case SDL_SCANCODE_SPACE:
		{
			/*flip *= -1.0f;
			gameBoardTC = gameboard->GetComponent<TransformComponent>();
			Quaternion start = gameBoardTC->GetQuaternion();
			Quaternion end = QMath::angleAxisRotation(180.0f * flip, Vec3(0.0f, 0.0f, 1.0f)) * start;
			gameBoardTC->SlerpOrientation(start, end, 3.0f);*/
			break;
		}
		}


		switch (sdlEvent.key.keysym.scancode) {
		case SDL_SCANCODE_E:
			cameraTC->SetTransform(cameraTC->GetPosition(), cameraTC->GetQuaternion() *
				QMath::angleAxisRotation(-2.0f, Vec3(0.0f, 1.0f, 0.0f)));
			break;

		case SDL_SCANCODE_Q:
			cameraTC->SetTransform(cameraTC->GetPosition(), cameraTC->GetQuaternion() *
				QMath::angleAxisRotation(2.0f, Vec3(0.0f, 1.0f, 0.0f)));
			break;


		case SDL_SCANCODE_A:
			goLeft = true;
			break;
		case SDL_SCANCODE_W:
			goBackwards = true;
			break;
		case SDL_SCANCODE_D:
			goRight = true;
			break;
		case SDL_SCANCODE_S:
			goForward = true;
			break;


		case SDL_SCANCODE_N:
			if (drawNormals == false) drawNormals = true;

			else drawNormals = false;
			break;

		case SDL_SCANCODE_O:
			if (drawOverlay == false) drawOverlay = true;
			else drawOverlay = false;
			break;

		default:
			break;
		}
		break;

	case SDL_KEYUP:
		switch (sdlEvent.key.keysym.scancode) {
		case SDL_SCANCODE_A:
			goLeft = false;
			break;
		case SDL_SCANCODE_W:
			goBackwards = false;
			break;
		case SDL_SCANCODE_D:
			goRight = false;
			break;
		case SDL_SCANCODE_S:
			goForward = false;
			break;
		default:
			break;

		}
	case SDL_MOUSEBUTTONDOWN:
		if (sdlEvent.button.button == (SDL_BUTTON_RIGHT)) {
			rotatePlayerRight = true;
			break;
		}
		if (sdlEvent.button.button == (SDL_BUTTON_LEFT)) {
			rotatePlayerLeft = true;

			break;
		}
		break;

	case SDL_MOUSEBUTTONUP:
	{
		if (sdlEvent.button.button == (SDL_BUTTON_RIGHT)) {
			rotatePlayerRight = false;
			break;
		}
		if (sdlEvent.button.button == (SDL_BUTTON_LEFT)) {
			rotatePlayerLeft = false;
			break;
		}
		break;
	}
	}
}

void ShaderTestScene::Update(const float deltaTime) {
	iTime += deltaTime;

	camera->UpdateViewMatrix();


	
	cube->GetComponent<TransformComponent>()->Update(deltaTime);


}


void ShaderTestScene::Render() const {
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.2f, 0.2f, 0.2f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindBuffer(GL_UNIFORM_BUFFER, camera->GetMatriciesID());
	glBindBuffer(GL_UNIFORM_BUFFER, light->GetLightID());

	glUseProgram(cube->GetComponent<ShaderComponent>()->GetProgram());
	glUniform1f(cube->GetComponent<ShaderComponent>()->GetUniformID("index"), animIndex);
	glUniformMatrix4fv(cube->GetComponent<ShaderComponent>()->GetUniformID("modelMatrix"), 1, GL_FALSE, cube->GetModelMatrix());
	glBindTexture(GL_TEXTURE_2D, cube->GetComponent<MaterialComponent>()->getTextureID());
	cube->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);



	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

	if (drawOverlay == true) {
		DrawMeshOverlay(Vec4(1.0f, 1.0f, 1.0f, 0.5f));
	}

	if (drawNormals == true) {
		DrawNormals(Vec4(1.0f, 1.0f, 0.0f, 0.05f));
	}
}



void ShaderTestScene::DrawNormals(const Vec4 color) const {
	glBindBuffer(GL_UNIFORM_BUFFER, camera->GetMatriciesID());
	Ref<ShaderComponent> shader = assetManager->GetComponent<ShaderComponent>("DrawNormalsShader");
	glUseProgram(shader->GetProgram());
	glUniform4fv(shader->GetUniformID("color"), 1, color);
	for (auto actor : actors) {
		glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, actor->GetModelMatrix());
		actor->GetComponent<MeshComponent>()->Render();
	}
	glUseProgram(0);
}



void ShaderTestScene::DrawMeshOverlay(const Vec4 color) const {
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindBuffer(GL_UNIFORM_BUFFER, camera->GetMatriciesID());
	Ref<ShaderComponent> shader = assetManager->GetComponent<ShaderComponent>("DefaultShader");
	glUseProgram(shader->GetProgram());
	glUniform4fv(shader->GetUniformID("color"), 1, color);

	for (auto actor : actors) {
		glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, actor->GetModelMatrix());
		actor->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
	}
	glUseProgram(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


