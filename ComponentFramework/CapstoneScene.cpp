#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Debug.h"
#include "CapstoneScene.h"
#include "MMath.h""
#include <Plane.h>
//#include <PMath.h>

#include "Debug.h"
#include "CameraActor.h"
#include "LightActor.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "MaterialComponent.h"
#include "SkyBox.h"
#include "Room.h"
#include <string>
using namespace MATH;
//using namespace MATHEX;

//BEDROOM SCENE
CapstoneScene::CapstoneScene() :drawNormals(false), drawOverlay(false) {
	Debug::Info("Created Scene Bedroom: ", __FILE__, __LINE__);
	glGetIntegerv(GL_VIEWPORT, viewport);
}

bool CapstoneScene::OnCreate() {
	Debug::Info("Loading assets Scene Dream: ", __FILE__, __LINE__);
	assetManager = std::make_shared<AssetManager>();

	Ref<ShaderComponent> shader = assetManager->GetComponent<ShaderComponent>("TextureShader");
	Ref<ShaderComponent> regularTextureShader = assetManager->GetComponent<ShaderComponent>("RefularTexture");

	player = std::make_shared<Actor>(nullptr);
	Quaternion orientation = QMath::angleAxisRotation(0.0f, Vec3(1.0f, 0.0f, 0.0f));
	player->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, 0.0f), orientation);
	//player->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Square"));
	//player->AddComponent<ShaderComponent>(shader);
//	player->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Sprite_Sheet_JPG"));

	AddActor(player);
	
	camera = std::make_shared<CameraActor>(player.get());
	camera->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, -6.8f), Quaternion());
	camera->OnCreate();
	camera->GetProjectionMatrix().print("ProjectionMatrix");
	camera->GetViewMatrix().print("ViewMatrix");

	
	doll = std::make_shared<Actor>(nullptr);
	doll->AddComponent<TransformComponent>(nullptr, Vec3(3.99f, 0.94f, -1.6f), QMath::angleAxisRotation(270.0f, Vec3(0.0f, 1.0f, 0.0f)));
	doll->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Square"));
	doll->AddComponent<ShaderComponent>(regularTextureShader);
	doll->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Doll"));
	
	

	room = std::make_shared<Room>(nullptr, "textures/Wall2_new.png", "textures/Wall1.png",
		"textures/Ceiling.png", "textures/Floor.png", "textures/Wall4.png",
		"textures/Wall3_new.png", 4.0f);

	room->OnCreate();

	light = std::make_shared<LightActor>(camera.get(), LightStyle::DirectionLight, Vec3(0.0f, 5.0f, 1.0f), Vec4(0.85f, 0.6, 0.6f, 0.0f));
	light->OnCreate();

	return true;
}





CapstoneScene::~CapstoneScene() {
	Debug::Info("Deleted Scene Bedroom: ", __FILE__, __LINE__);
	OnDestroy();
}

void CapstoneScene::OnDestroy() {
	Debug::Info("Deleting assets Scene Bedroom: ", __FILE__, __LINE__);
}

void CapstoneScene::HandleEvents(const SDL_Event& sdlEvent) {
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

void CapstoneScene::Update(const float deltaTime) {

	
	
	if (goRight) {
		currentTime += deltaTime;
		animIndex = static_cast<int>(currentTime / frameSpeed) % 8;  
		
		player->GetComponent<TransformComponent>()->SetPosition(player->GetComponent<TransformComponent>()->GetPosition()
			+ MMath::toMatrix4(player->GetComponent<TransformComponent>()->GetQuaternion()) * lefr_right_Vector);
	}
	if (goLeft) {
		currentTime += deltaTime;
		animIndex = static_cast<int>(currentTime / frameSpeed) % 8;

		player->GetComponent<TransformComponent>()->SetPosition(player->GetComponent<TransformComponent>()->GetPosition()
			- MMath::toMatrix4(player->GetComponent<TransformComponent>()->GetQuaternion()) * lefr_right_Vector);
	}
	if (goForward) {
		currentTime += deltaTime;
		animIndex = static_cast<int>(currentTime / frameSpeed) % 8;

		player->GetComponent<TransformComponent>()->SetPosition(player->GetComponent<TransformComponent>()->GetPosition()
			+ MMath::toMatrix4(player->GetComponent<TransformComponent>()->GetQuaternion()) * forwardVector);
	}

	if (goBackwards) {
		currentTime += deltaTime;
		animIndex = static_cast<int>(currentTime / frameSpeed) % 8;

		player->GetComponent<TransformComponent>()->SetPosition(player->GetComponent<TransformComponent>()->GetPosition()
			- MMath::toMatrix4(player->GetComponent<TransformComponent>()->GetQuaternion()) * forwardVector);
	}

	if (rotatePlayerLeft) {

		playerAngle += 0.4f;
		Quaternion newQuat = QMath::angleAxisRotation(playerAngle, Vec3(0.0f, 1.0f, 0.0f));

		player->GetComponent<TransformComponent>()->SetQuaternion(newQuat);
	}

	if (rotatePlayerRight) {

		playerAngle -= 0.4f;
		Quaternion newQuat = QMath::angleAxisRotation(playerAngle, Vec3(0.0f, 1.0f, 0.0f));

		player->GetComponent<TransformComponent>()->SetQuaternion(newQuat);
	}
	camera->UpdateViewMatrix();
	player->GetComponent<TransformComponent>()->Update(deltaTime);
}




void CapstoneScene::Render() const {
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.2f, 0.2f, 0.2f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindBuffer(GL_UNIFORM_BUFFER, camera->GetMatriciesID());
	glBindBuffer(GL_UNIFORM_BUFFER, light->GetLightID());
	
	//Ref<ShaderComponent> skyboxShader = skybox->GetComponent<ShaderComponent>();

	//glUseProgram(skyboxShader->GetProgram());
	//glUniformMatrix4fv(skyboxShader->GetUniformID("modelMatrix"), 1, GL_FALSE, skybox->GetModelMatrix());
	//glUniformMatrix4fv(skyboxShader->GetUniformID("viewMatrix"), 1, GL_FALSE, MMath::inverse(camera->orient));

	////camera->orient.print("Camera orientation");
	//glUniformMatrix4fv(skyboxShader->GetUniformID("projectionMatrix"), 1, GL_FALSE, camera->GetProjectionMatrix());
	//std::dynamic_pointer_cast<SkyBox>(skybox)->Render();
	//glUseProgram(0);

	Ref<ShaderComponent> roomShader = room->GetComponent<ShaderComponent>();
	glUseProgram(roomShader->GetProgram());
	glUniformMatrix4fv(roomShader->GetUniformID("modelMatrix"), 1, GL_FALSE, room->GetModelMatrix());
	glUniformMatrix4fv(roomShader->GetUniformID("viewMatrix"), 1, GL_FALSE, camera->GetViewMatrix());
	glUniformMatrix4fv(roomShader->GetUniformID("projectionMatrix"), 1, GL_FALSE, camera->GetProjectionMatrix());
	std::dynamic_pointer_cast<Room>(room)->Render();
	glUseProgram(0);




	// Then render player with proper depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);  // Only render if closer than existing geometry


	glUseProgram(doll->GetComponent<ShaderComponent>()->GetProgram());
	glUniformMatrix4fv(doll->GetComponent<ShaderComponent>()->GetUniformID("modelMatrix"), 1, GL_FALSE, doll->GetModelMatrix());
	glUniformMatrix4fv(doll->GetComponent<ShaderComponent>()->GetUniformID("viewMatrix"), 1, GL_FALSE, camera->GetViewMatrix());
//	glUniformMatrix4fv(roomShader->GetUniformID("projectionMatrix"), 1, GL_FALSE, camera->GetProjectionMatrix());
	glBindTexture(GL_TEXTURE_2D, doll->GetComponent<MaterialComponent>()->getTextureID());
	doll->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
	glUseProgram(0);


	for (auto actor : actors) {
		if (actor->GetComponent<ShaderComponent>() != nullptr) {
			glUseProgram(actor->GetComponent<ShaderComponent>()->GetProgram());
			glUniform1f(actor->GetComponent<ShaderComponent>()->GetUniformID("index"), animIndex);
			glUniformMatrix4fv(actor->GetComponent<ShaderComponent>()->GetUniformID("modelMatrix"), 1, GL_FALSE, actor->GetModelMatrix());
			glBindTexture(GL_TEXTURE_2D, actor->GetComponent<MaterialComponent>()->getTextureID());
			actor->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
		}
	
	}



	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

	if (drawOverlay == true) {
		DrawMeshOverlay(Vec4(1.0f, 1.0f, 1.0f, 0.5f));
	}

	if (drawNormals == true) {
		DrawNormals(Vec4(1.0f, 1.0f, 0.0f, 0.05f));
	}
}



void CapstoneScene::DrawNormals(const Vec4 color) const {
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



void CapstoneScene::DrawMeshOverlay(const Vec4 color) const {
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


