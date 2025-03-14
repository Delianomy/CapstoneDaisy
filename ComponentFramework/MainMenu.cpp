#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Debug.h"
#include "MainMenu.h"
#include "MMath.h"
#include "Debug.h"
#include "CameraActor.h"
#include "LightActor.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "MaterialComponent.h"
#include "PhysicsComponent.h"
#include "CollisionComponent.h"


#include "SkyBox.h"
#include "Room.h"
#include <string>


using namespace MATH;


MainMenu::MainMenu(SceneManager* scenemanager) :drawNormals(false), drawOverlay(false) {
	Debug::Info("Created Scene Dream: ", __FILE__, __LINE__);
	glGetIntegerv(GL_VIEWPORT, viewport);
	this->sceneManagerRef = scenemanager;
}

bool MainMenu::OnCreate() {
	Debug::Info("Loading assets Scene MAIN MENU: ", __FILE__, __LINE__);
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
	camera->isInMainMenu = true;
	camera->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, -2.0f), Quaternion());
	camera->OnCreate();


	//SKYBOX OPTION
	skybox = std::make_shared<SkyBox>(nullptr, "textures/Skyboxes/MainMenu/px.png", "textures/Skyboxes/MainMenu/nx.png",
		"textures/Skyboxes/MainMenu/py.png", "textures/Skyboxes/MainMenu/ny.png", "textures/Skyboxes/MainMenu/pz.png",
		"textures/Skyboxes/MainMenu/nz.png");

	skybox->OnCreate();

	light = std::make_shared<LightActor>(camera.get(), LightStyle::DirectionLight, Vec3(0.0f, 5.0f, 1.0f), Vec4(0.85f, 0.6, 0.6f, 0.0f));
	light->OnCreate();

	audioManager.PlaySound("ost/1.mp3");

	return true;
}




MainMenu::~MainMenu() {
	Debug::Info("Deleted Scene Dream: ", __FILE__, __LINE__);

	OnDestroy();
}

void MainMenu::OnDestroy() {
	Debug::Info("Deleting Scene Dream: ", __FILE__, __LINE__);
}

void MainMenu::HandleEvents(const SDL_Event& sdlEvent) {
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

		case SDL_SCANCODE_SPACE:
			break;


		case SDL_SCANCODE_Q:
			cameraTC->SetTransform(cameraTC->GetPosition(), cameraTC->GetQuaternion() *
				QMath::angleAxisRotation(2.0f, Vec3(0.0f, 1.0f, 0.0f)));
			break;
		case SDL_SCANCODE_E:
			inventoryButtonPressed = !inventoryButtonPressed;
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
	ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
	}
}


void MainMenu::Update(const float deltaTime) {


	DrawUI_imgui();

	playerAngle += 0.05f;
	Quaternion newQuat = QMath::angleAxisRotation(playerAngle, Vec3(0.0f, 1.0f, 0.0f));
	player->GetComponent<TransformComponent>()->SetQuaternion(newQuat);
	

	
	camera->UpdateViewMatrix();
	player->GetComponent<TransformComponent>()->Update(deltaTime);

}


void MainMenu::Render() const {
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.2f, 0.2f, 0.2f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindBuffer(GL_UNIFORM_BUFFER, camera->GetMatriciesID());
	glBindBuffer(GL_UNIFORM_BUFFER, light->GetLightID());



	//SKYBOX OPTION

	Ref<ShaderComponent> skyboxShader = skybox->GetComponent<ShaderComponent>();

	glUseProgram(skyboxShader->GetProgram());
	glUniformMatrix4fv(skyboxShader->GetUniformID("modelMatrix"), 1, GL_FALSE, skybox->GetModelMatrix());
	glUniformMatrix4fv(skyboxShader->GetUniformID("viewMatrix"), 1, GL_FALSE, MMath::inverse(camera->orient));

	//camera->orient.print("Camera orientation");
	glUniformMatrix4fv(skyboxShader->GetUniformID("projectionMatrix"), 1, GL_FALSE, camera->GetProjectionMatrix());
	std::dynamic_pointer_cast<SkyBox>(skybox)->Render();
	glUseProgram(0);






	// Then render player with proper depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);  // Only render if closer than existing geometry
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (auto opaqueActor : opaqueActors) {
		glUseProgram(opaqueActor->GetComponent<ShaderComponent>()->GetProgram());
		glUniform1f(opaqueActor->GetComponent<ShaderComponent>()->GetUniformID("index"), animIndex);
		glUniformMatrix4fv(opaqueActor->GetComponent<ShaderComponent>()->GetUniformID("modelMatrix"), 1, GL_FALSE, opaqueActor->GetModelMatrix());
		glBindTexture(GL_TEXTURE_2D, opaqueActor->GetComponent<MaterialComponent>()->getTextureID());
		opaqueActor->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
	}


	for (auto transparentActor : transparentActors) {
		glUseProgram(transparentActor->GetComponent<ShaderComponent>()->GetProgram());
		glUniform1f(transparentActor->GetComponent<ShaderComponent>()->GetUniformID("index"), animIndex);
		glUniformMatrix4fv(transparentActor->GetComponent<ShaderComponent>()->GetUniformID("modelMatrix"), 1, GL_FALSE, transparentActor->GetModelMatrix());
		glBindTexture(GL_TEXTURE_2D, transparentActor->GetComponent<MaterialComponent>()->getTextureID());
		transparentActor->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
	}

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}




void MainMenu::DrawUI_imgui()
{
	int windowHeight = sceneManagerRef->getWindowHeight();
	int windowWidth = sceneManagerRef->getWindowWidth();


	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();


	//Inventory 
	ImGui::SetNextWindowPos(ImVec2(620, 150)); // Set the window position here (x, y)
	ImGui::SetNextWindowSize(ImVec2(700, 400)); // Set the window size

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::Begin("Game Title", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration);
	//ImGui::SetCursorPos(ImVec2(-400, 400));
	//draw image
	ImGui::Image((intptr_t)assetManager->GetComponent<MaterialComponent>("Game Title")->getTextureID(), ImVec2(700, 400));
	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::PopStyleColor();


	//Credits 
	ImGui::SetNextWindowPos(ImVec2(620, 150)); // Set the window position here (x, y)
	ImGui::SetNextWindowSize(ImVec2(700, 400)); // Set the window size

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::Begin("Game Title", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration);
	//ImGui::SetCursorPos(ImVec2(-400, 400));
	//draw image
	ImGui::Image((intptr_t)assetManager->GetComponent<MaterialComponent>("Game Title")->getTextureID(), ImVec2(700, 400));
	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::PopStyleColor();


}




