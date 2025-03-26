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
	camera->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, -3.8f), Quaternion());
	camera->OnCreate();
	camera->GetProjectionMatrix().print("ProjectionMatrix");
	camera->GetViewMatrix().print("ViewMatrix");

	
	Quaternion dollRot = QMath::angleAxisRotation(90.0f, Vec3(0.0f, 1.0f, 0.0f));
	doll = std::make_shared<Actor>(nullptr);
	doll->AddComponent<TransformComponent>(nullptr, Vec3(3.99f, 0.94f, -1.6f), dollRot);
	doll->GetComponent<TransformComponent>()->SetScale(Vec3(1.0f, 1.0f, 1.0f));
	doll->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Square"));
	doll->AddComponent<ShaderComponent>(shader);
	doll->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Sprite_Sheet_HEAVY"));
	AddTransparentActor(doll);



	//doll = std::make_shared<Actor>(nullptr);
	//doll->AddComponent<TransformComponent>(nullptr, Vec3(3.99f, 0.94f, -1.6f), QMath::angleAxisRotation(270.0f, Vec3(0.0f, 1.0f, 0.0f)));
	//doll->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Square"));
	//doll->AddComponent<ShaderComponent>(regularTextureShader);
	//doll->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Doll"));
	//AddTransparentActor(doll);
	

	room = std::make_shared<Room>(nullptr, "textures/SkyBoxes/Room/1.png", "textures/SkyBoxes/Room/2.png",
		"textures/SkyBoxes/Room/celling.png", "textures/SkyBoxes/Room/floor.png", "textures/SkyBoxes/Room/3.png",
		"textures/SkyBoxes/Room/4.png", 4.0f);

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
	auto test = &CapstoneScene::Update;
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
				rotatePlayerLeft = true;

				break;
			case SDL_SCANCODE_D:
				rotatePlayerRight = true;
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
			rotatePlayerLeft = false;
			break;

		case SDL_SCANCODE_D:
			rotatePlayerRight = false;
			break;

		default:
			break;

		}
	case SDL_MOUSEBUTTONDOWN:
		if (sdlEvent.button.button == (SDL_BUTTON_RIGHT)) {
	
		}
		if (sdlEvent.button.button == (SDL_BUTTON_LEFT)) {
		
			currentMousePos = Vec2(static_cast<float>(sdlEvent.button.x), static_cast<float>(sdlEvent.button.y));
			lastMousePos = currentMousePos;
			objID = Pick(sdlEvent.button.x, sdlEvent.button.y);
			printf("0x%X %d\n", objID, objID);
			break;
		}
		break;

	case SDL_MOUSEBUTTONUP:
	{
		if (sdlEvent.button.button == (SDL_BUTTON_RIGHT)) {
		
			break;
		}
		if (sdlEvent.button.button == (SDL_BUTTON_LEFT)) {
		
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


int CapstoneScene::Pick(int x, int y) {
	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // White background with alpha=1
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Get and use the color picker shader
	Ref<ShaderComponent> shader = assetManager->GetComponent<ShaderComponent>("ColorPicker");
	glUseProgram(shader->GetProgram());

	// Set matrices
	glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, camera->GetProjectionMatrix());
	glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, camera->GetViewMatrix());

	// Render opaque actors with unique colors
	for (GLuint i = 0; i < opaqueActors.size(); i++) {
		glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, opaqueActors[i]->GetModelMatrix());
		glUniform1ui(shader->GetUniformID("colorID"), i + 1); // Start from 1 to avoid black
		opaqueActors[i]->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
	}

	// Render transparent actors with unique colors
	for (GLuint i = 0; i < transparentActors.size(); i++) {
		glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, transparentActors[i]->GetModelMatrix());
		glUniform1ui(shader->GetUniformID("colorID"), opaqueActors.size() + i + 1); // Continue numbering from opaque actors
		transparentActors[i]->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
	}

	// Unbind shader
	glUseProgram(0);

	// Read pixel color at mouse position
	unsigned char pixel[4];
	glReadPixels(x, viewport.height - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);

	// Convert RGB to color index
	GLuint colorIndex = pixel[0] + (pixel[1] << 8) + (pixel[2] << 16);

	// Debug output
	printf("Read pixel color: R=%d, G=%d, B=%d, Index=0x%06X\n",
		pixel[0], pixel[1], pixel[2], colorIndex);

	// Check if background was clicked (white)
	if (colorIndex == 0xFFFFFF) {
		return -1;
	}
	else {
		return colorIndex - 1; // Subtract 1 to get back to 0-based index
	}
}

void CapstoneScene::Render() const {
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.2f, 0.2f, 0.2f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindBuffer(GL_UNIFORM_BUFFER, camera->GetMatriciesID());
	glBindBuffer(GL_UNIFORM_BUFFER, light->GetLightID());
	

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



	/*for (auto transparentActor : transparentActors) {
		glUseProgram(transparentActor->GetComponent<ShaderComponent>()->GetProgram());
		glUniform1f(transparentActor->GetComponent<ShaderComponent>()->GetUniformID("index"), animIndex);
		glUniformMatrix4fv(transparentActor->GetComponent<ShaderComponent>()->GetUniformID("modelMatrix"), 1, GL_FALSE, transparentActor->GetModelMatrix());
		if (transparentActor->GetComponent<MaterialComponent>()->getTextureID()) {
			glBindTexture(GL_TEXTURE_2D, transparentActor->GetComponent<MaterialComponent>()->getTextureID());
			transparentActor->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
		}
	}*/



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


