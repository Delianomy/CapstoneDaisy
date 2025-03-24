#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Debug.h"
#include "SandboxAdriel.h"
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
#include "TriggerComponent.h"
#include "SkyBox.h"
#include <string>
using namespace MATH;


SandboxAdriel::SandboxAdriel(SceneManager* scenemanager) :drawNormals(false), drawOverlay(false) {
	Debug::Info("Created Scene Dream: ", __FILE__, __LINE__);
	glGetIntegerv(GL_VIEWPORT, viewport);
	this->sceneManagerRef = scenemanager;
}

bool SandboxAdriel::OnCreate() {
	Debug::Info("Loading assets Scene Dream: ", __FILE__, __LINE__);
	assetManager = std::make_shared<AssetManager>();
	inventory = new Inventory();

	//Main Shaders
	Ref<ShaderComponent> shader = assetManager->GetComponent<ShaderComponent>("TextureShader");
	Ref<ShaderComponent> WaveShader = assetManager->GetComponent<ShaderComponent>("WaveShader");
	Ref<ShaderComponent> CubeShader = assetManager->GetComponent<ShaderComponent>("RegularTextureShader");

	//Creating the debug objects
	DebugSphere = std::make_shared<Actor>(nullptr);
	DebugSphere->AddComponent<TransformComponent>(nullptr, Vec3(), Quaternion(), Vec3());
	DebugSphere->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Sphere"));
	DebugSphere->AddComponent<ShaderComponent>(assetManager->GetComponent< ShaderComponent>("DefaultShader"));

	DebugCube = std::make_shared<Actor>(nullptr);
	DebugCube->AddComponent<TransformComponent>(nullptr, Vec3(), Quaternion(), Vec3());
	DebugCube->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Cube"));
	DebugCube->AddComponent<ShaderComponent>(assetManager->GetComponent< ShaderComponent>("DefaultShader"));


	//Creating the player
	player = std::make_shared<Actor>(nullptr);
	player->AddComponent<PhysicsComponent>(
		nullptr, //Parent
		Vec3(0.0f, 0.0f, 0.0f),//pos
		Quaternion(), //Rotation
		Vec3(0.0f, 0.0f, 0.0f) //velocity
	);
	player->GetComponent<PhysicsComponent>()->SetScale(Vec3(1.0f, 1.0f, 1.0f));
	player->GetComponent<PhysicsComponent>()->isStatic = false;
	player->GetComponent<PhysicsComponent>()->useGravity = true;
	player->GetComponent<PhysicsComponent>()->mass = 5.0f;
	player->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Square"));
	player->AddComponent<ShaderComponent>(shader);
	player->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("ChessBoard"));
	player->AddComponent<CollisionComponent>(nullptr, 0.5f);
	player->AddComponent<TriggerComponent>(nullptr, 1.0f);
	player->GetComponent<TriggerComponent>()->SetCallback(TriggerCallbackCreator::CreateTriggerCallback(this, &SandboxAdriel::AddItemToInventory));
	AddTransparentActor(player);

	//Creating the camera
	camera = std::make_shared<CameraActor>(player.get());
	camera->isInMainMenu = false;
	camera->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, -5.0f), Quaternion());
	camera->OnCreate();

	//Creating the floor
	cube = std::make_shared<Actor>(nullptr);
	cube->AddComponent<PhysicsComponent>(
		nullptr, //Parent
		Vec3(0.0f, -2.0f, 0.0f), //pos
		Quaternion(), //Rotation
		Vec3(0.0f, 0.0f, 0.0f) //velocity
	);
	cube->GetComponent<PhysicsComponent>()->SetScale(Vec3(4.0f, 0.5f, 1.0f));
	cube->GetComponent<PhysicsComponent>()->isStatic = true;
	cube->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Cube"));
	AABB col = AABB(cube->GetComponent<PhysicsComponent>()->GetPosition(), Vec3(4.0f, 0.5f, 1.0f));
	cube->AddComponent<CollisionComponent>(nullptr, col);
	cube->AddComponent<ShaderComponent>(CubeShader);
	cube->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("ChessBoard"));
	AddOpaqueActor(cube);


	skybox = std::make_shared<SkyBox>(nullptr, "textures/px.png", "textures/nx.png",
		"textures/py.png", "textures/ny.png", "textures/pz.png",
		"textures/nz.png");

	skybox->OnCreate();

	light = std::make_shared<LightActor>(camera.get(), LightStyle::DirectionLight, Vec3(0.0f, 5.0f, 1.0f), Vec4(0.85f, 0.6, 0.6f, 0.0f));
	light->OnCreate();

	Ref<PickableItem> newPickable = std::make_shared<PickableItem>(assetManager, "Item1", Vec3(5.0f,-1.0f,0.0f));
	triggerSystem.AddActor(newPickable);
	opaqueActors.push_back(newPickable);

	physicsSystem.AddActor(player);
	physicsSystem.AddActor(cube);
	
	collisionSystem.AddActor(player);
	collisionSystem.AddActor(cube);

	triggerSystem.AddActor(player);

	return true;
}

SandboxAdriel::~SandboxAdriel() {
	Debug::Info("Deleted Scene Dream: ", __FILE__, __LINE__);

	OnDestroy();
}

void SandboxAdriel::OnDestroy() {
	Debug::Info("Deleting Scene Dream: ", __FILE__, __LINE__);
}

void SandboxAdriel::HandleEvents(const SDL_Event& sdlEvent) {
	static int objID = -1;
	static Vec2 currentMousePos;
	static Vec2	lastMousePos;
	static float flip = 1.0f;
	Ref<TransformComponent> cameraTC;
	Ref<TransformComponent> gameBoardTC;
	ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
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
	}
}

void SandboxAdriel::Update(const float deltaTime) {
	
	
	DrawUI_imgui();
	iTime += deltaTime;

	//change in angle 
	if (rotatePlayerLeft) {
		playerAngle -= 0.4f;
	}
	else if (rotatePlayerRight) {
		playerAngle += 0.4f;
	}

	Ref<PhysicsComponent> playerPhysics = player->GetComponent<PhysicsComponent>();

	//Update players rotation 
	if(rotatePlayerLeft || rotatePlayerRight){
		Quaternion newQuat = QMath::angleAxisRotation(playerAngle, Vec3(0.0f, 1.0f, 0.0f));
		playerPhysics->SetQuaternion(newQuat);
	}
	



	Vec3 movement(0.0f, 0.0f, 0.0f);
	if (goRight) {
		
		movement.x += 1.0f;
	}
	if (goLeft) {
		movement.x -= 1.0f;
	}
	if (goForward) {
		movement.z += 1.0f;
	}
	if (goBackwards) {
		movement.z -= 1.0f;
	}

	//normalize the movement to remove the diagonal speed up
	if (VMath::mag(movement) > 0.0f) {
		VMath::normalize(movement);
		currentTime += deltaTime;
		animIndex = static_cast<int>(currentTime / frameSpeed) % 13;
		Quaternion currentRot = playerPhysics->GetQuaternion();
		Matrix4 currentRotMatrix = MMath::toMatrix4(currentRot);
		Vec3 moveDirection = Vec3();
		if (movement.x != 0) {
			moveDirection += currentRotMatrix * lefr_right_Vector * movement.x * walkSpeed;
		}
		if (movement.z != 0.0f) {
			moveDirection += currentRotMatrix * forwardVector * movement.z * walkSpeed;
		}
		playerPhysics->SetVel(moveDirection);
	}
	else {
		playerPhysics->SetVel(Vec3());
		animIndex = 0;
	}
		


	camera->UpdateViewMatrix();
	collisionSystem.Update(deltaTime);
	physicsSystem.Update(deltaTime);
	triggerSystem.Update(deltaTime);

}

void SandboxAdriel::Render() const{
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindBuffer(GL_UNIFORM_BUFFER, camera->GetMatriciesID());
	glBindBuffer(GL_UNIFORM_BUFFER, light->GetLightID());

	Ref<ShaderComponent> skyboxShader = skybox->GetComponent<ShaderComponent>();

	glUseProgram(skyboxShader->GetProgram());
	glUniformMatrix4fv(skyboxShader->GetUniformID("modelMatrix"), 1, GL_FALSE, skybox->GetModelMatrix());
	glUniformMatrix4fv(skyboxShader->GetUniformID("viewMatrix"), 1, GL_FALSE, MMath::inverse(camera->orient));
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

	//Drawing all the pickable objects
	for (auto trigger : triggerSystem.triggeringActors) {
		DrawSphere(trigger->GetComponent<TransformComponent>()->GetPosition(), trigger->GetComponent<TriggerComponent>()->radius);
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

void SandboxAdriel::DrawUI_imgui()
{
	int windowHeight = sceneManagerRef->getWindowHeight();
	int windowWidth = sceneManagerRef->getWindowWidth();

	GLuint inventoryTextureID;
	if (inventoryButtonPressed) {
		inventoryTextureID = assetManager->GetComponent<MaterialComponent>("inventory_closed")->getTextureID();
	}
	else {
		inventoryTextureID = assetManager->GetComponent<MaterialComponent>("inventory_open")->getTextureID();
	}

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();


	DebugUI();


	ImGui::SetNextWindowPos(ImVec2(220, 750)); // Set the window position here (x, y)
	ImGui::SetNextWindowSize(ImVec2(220, 220)); // Set the window size

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::Begin("inventory", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration);
	//ImGui::SetCursorPos(ImVec2(-400, 400));
	//draw image
	ImGui::Image((intptr_t)inventoryTextureID, ImVec2(200, 200));
	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::PopStyleColor();



	if (inventoryButtonPressed) {
		//E - button
		ImGui::SetNextWindowPos(ImVec2(360, 860)); // Set the window position here (x, y)
		ImGui::SetNextWindowSize(ImVec2(120, 120)); // Set the window size

		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::Begin("E button", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration);
		//draw image
		ImGui::Image((intptr_t)assetManager->GetComponent<MaterialComponent>("E button")->getTextureID(), ImVec2(99, 99));
		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();

	}
	if (!inventoryButtonPressed) {
	
		/// ---------------------------INVENTORY SLOT 1 ----------------------------------------------------------------------
		ImGui::SetNextWindowPos(ImVec2(250, 590)); // Set the window position here (x, y)
		ImGui::SetNextWindowSize(ImVec2(150, 150)); // Set the window size
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::Begin("inventory slot 1", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration);
		ImGui::Image((intptr_t)assetManager->GetComponent<MaterialComponent>("inventory slot")->getTextureID(), ImVec2(140, 140));
		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();

		ImGui::SetNextWindowPos(ImVec2(330, 660)); // Set the window position here (x, y)
		ImGui::SetNextWindowSize(ImVec2(80, 80)); // Set the window size
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::Begin("1 button", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration);
		ImGui::Image((intptr_t)assetManager->GetComponent<MaterialComponent>("number 1")->getTextureID(), ImVec2(80, 80));
		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();




		/// ---------------------------INVENTORY SLOT 2 ----------------------------------------------------------------------

		ImGui::SetNextWindowPos(ImVec2(395, 665)); // Set the window position here (x, y)
		ImGui::SetNextWindowSize(ImVec2(150, 150)); // Set the window size
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::Begin("inventory slot 2", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration);
		ImGui::Image((intptr_t)assetManager->GetComponent<MaterialComponent>("inventory slot")->getTextureID(), ImVec2(140, 140));
		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();

		ImGui::SetNextWindowPos(ImVec2(475, 735)); // Set the window position here (x, y)
		ImGui::SetNextWindowSize(ImVec2(80, 80)); // Set the window size
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::Begin("2 button", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration);
		ImGui::Image((intptr_t)assetManager->GetComponent<MaterialComponent>("number 2")->getTextureID(), ImVec2(80, 80));
		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();


		/// ---------------------------INVENTORY SLOT 3 ----------------------------------------------------------------------
		ImGui::SetNextWindowPos(ImVec2(465, 815)); // Set the window position here (x, y)
		ImGui::SetNextWindowSize(ImVec2(150, 150)); // Set the window size
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::Begin("inventory slot 3", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration);
		ImGui::Image((intptr_t)assetManager->GetComponent<MaterialComponent>("inventory slot")->getTextureID(), ImVec2(140, 140));
		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();


		ImGui::SetNextWindowPos(ImVec2(545, 885)); // Set the window position here (x, y)
		ImGui::SetNextWindowSize(ImVec2(80, 80)); // Set the window size
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::Begin("3 button", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration);
		ImGui::Image((intptr_t)assetManager->GetComponent<MaterialComponent>("number 3")->getTextureID(), ImVec2(80, 80));
		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();

	}




	
}

void SandboxAdriel::DrawSphere(Vec3 pos, float radius) const{
	DebugSphere->GetComponent<TransformComponent>()->SetPosition(pos);
	DebugSphere->GetComponent<TransformComponent>()->SetScale(Vec3(radius, radius, radius));

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glUseProgram(DebugSphere->GetComponent<ShaderComponent>()->GetProgram());
	glUniformMatrix4fv(DebugSphere->GetComponent<ShaderComponent>()->GetUniformID("projectionMatrix"), 1, GL_FALSE, camera->GetProjectionMatrix());
	glUniformMatrix4fv(DebugSphere->GetComponent<ShaderComponent>()->GetUniformID("viewMatrix"), 1, GL_FALSE, camera->GetViewMatrix());
	glUniformMatrix4fv(DebugSphere->GetComponent<ShaderComponent>()->GetUniformID("modelMatrix"), 1, GL_FALSE, DebugSphere->GetModelMatrix());

	DebugSphere->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
void SandboxAdriel::DrawSphere(Sphere s) const {
	DrawSphere(s.center, s.r);
}

void SandboxAdriel::DrawCube(Vec3 pos, Vec3 dimensions) const {
	DebugCube->GetComponent<TransformComponent>()->SetPosition(pos);
	DebugCube->GetComponent<TransformComponent>()->SetScale(dimensions);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glUseProgram(DebugCube->GetComponent<ShaderComponent>()->GetProgram());
	glUniformMatrix4fv(DebugCube->GetComponent<ShaderComponent>()->GetUniformID("projectionMatrix"), 1, GL_FALSE, camera->GetProjectionMatrix());
	glUniformMatrix4fv(DebugCube->GetComponent<ShaderComponent>()->GetUniformID("viewMatrix"), 1, GL_FALSE, camera->GetViewMatrix());
	glUniformMatrix4fv(DebugCube->GetComponent<ShaderComponent>()->GetUniformID("modelMatrix"), 1, GL_FALSE, DebugCube->GetModelMatrix());

	DebugCube->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
void SandboxAdriel::DrawCube(AABB a) const {
	DrawCube(a.center, Vec3(a.rx, a.ry, a.rz));
}

void SandboxAdriel::DrawNormals(const Vec4 color) const {
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

void SandboxAdriel::DrawMeshOverlay(const Vec4 color) const {
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

void SandboxAdriel::DebugUI() {
	float windowWidth = 1366;
	float windowHeight = 768;

	//Inventory 
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiCond_FirstUseEver;
	ImGui::SetNextWindowPos(ImVec2(windowWidth / 2 - windowWidth * 0.3f, windowHeight / 2));
	ImGui::SetNextWindowSize(ImVec2(200, 150));
	ImGui::Begin("Inventory", nullptr, flags);

	//Prints the item in the inventory
	ImGui::Text("%s", inventory->ToString().c_str());

	ImGui::End();
}

void SandboxAdriel::AddItemToInventory(std::shared_ptr<Actor> other) {
	//Check if it's a pickable item
	if (std::dynamic_pointer_cast<PickableItem>(other) != nullptr) {
		//Add the item to the inventory
		inventory->AddItem(std::dynamic_pointer_cast<PickableItem>(other));
		std::cout << other << "Added to inventory" << "\n";

		//Remove the item from the triggerSystem
		int index = 0;
		for (int i = 0; i < triggerSystem.triggeringActors.size(); i++) {
			if (triggerSystem.triggeringActors[i] == other) {
				index = i;
				break;
			}
		}
		triggerSystem.triggeringActors.erase(triggerSystem.triggeringActors.begin() + index);

		//Remove the item from the opaque actors
		/*index = 0;
		for (int i = 0; i < opaqueActors.size(); i++) {
			if (opaqueActors[i] == other) {
				index = i;
				break;
			}
		}
		opaqueActors.erase(opaqueActors.begin() + index);*/
	}
};