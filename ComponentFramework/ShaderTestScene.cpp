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
	
	Ref<ShaderComponent> WaveShader = assetManager->GetComponent<ShaderComponent>("SimpleWavesShader");

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
	auto meshComp = cube->GetComponent<MeshComponent>();
	cube->AddComponent<ShaderComponent>(WaveShader);
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
	
}

void ShaderTestScene::Update(const float deltaTime) {
	iTime += deltaTime;

	camera->UpdateViewMatrix();


	
	cube->GetComponent<TransformComponent>()->Update(deltaTime);


}


void ShaderTestScene::Render() const {
	//glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindBuffer(GL_UNIFORM_BUFFER, camera->GetMatriciesID());
	glBindBuffer(GL_UNIFORM_BUFFER, light->GetLightID());

	glUseProgram(cube->GetComponent<ShaderComponent>()->GetProgram());
	//glUniform1f(cube->GetComponent<ShaderComponent>()->GetUniformID("index"), animIndex);
	glUniform1f(cube->GetComponent<ShaderComponent>()->GetUniformID("iTime"), iTime);
	
	glUniformMatrix4fv(cube->GetComponent<ShaderComponent>()->GetUniformID("modelMatrix"), 1, GL_FALSE, cube->GetModelMatrix());

	glBindTexture(GL_TEXTURE_2D, cube->GetComponent<MaterialComponent>()->getTextureID());
	cube->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);



	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}





