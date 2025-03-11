#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Debug.h"
#include "CapstoneSceneDream.h"
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
#include <string>
using namespace MATH;


CapstoneSceneDream::CapstoneSceneDream(SceneManager* scenemanager) :drawNormals(false), drawOverlay(false) {
	Debug::Info("Created Scene Dream: ", __FILE__, __LINE__);
	glGetIntegerv(GL_VIEWPORT, viewport);
	this->sceneManagerRef = scenemanager;
}

bool CapstoneSceneDream::OnCreate() {
	Debug::Info("Loading assets Scene Dream: ", __FILE__, __LINE__);
	assetManager = std::make_shared<AssetManager>();

	Ref<ShaderComponent> shader = assetManager->GetComponent<ShaderComponent>("TextureShader");

	Ref<ShaderComponent> WaveShader = assetManager->GetComponent<ShaderComponent>("WaveShader");
	Ref<ShaderComponent> CubeShader = assetManager->GetComponent<ShaderComponent>("RefularTextureShader");

	//make an actor
	player = std::make_shared<Actor>(nullptr);
	player->AddComponent<PhysicsComponent>(nullptr, Vec3(0.0f, 4.0f, 0.0f),/// pos
		QMath::angleAxisRotation(0.0f, Vec3(1.0f, 0.0f, 0.0f)),
		Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	player->GetComponent<PhysicsComponent>()->SetScale(Vec3(2.0f, 2.0f, 4.0f));
	/// This makes a Sphere Collision Component because of the argument list - just the radius. 
	player->AddComponent<CollisionComponent>(nullptr, 1.0f);
	player->GetComponent<PhysicsComponent>()->isStatic = false;
	player->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Square"));
	player->AddComponent<ShaderComponent>(shader);
	player->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Sprite_Sheet_HEAVY"));
	AddTransparentActor(player);
	//AddActor(player);

	//now technically it would mean that out player now has a collider
	GLint maxTextureSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
	printf("Max Texture Size: %d\n", maxTextureSize);

	GLint totalMemoryKB;
	glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &totalMemoryKB);
	printf("Available GPU Memory: %d KB\n", totalMemoryKB);

	camera = std::make_shared<CameraActor>(player.get());
	camera->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, -3.5f), Quaternion());
	camera->OnCreate();
	camera->GetProjectionMatrix().print("ProjectionMatrix");
	camera->GetViewMatrix().print("ViewMatrix");



	skybox = std::make_shared<SkyBox>(nullptr, "textures/px.png", "textures/nx.png",
		"textures/py.png", "textures/ny.png", "textures/pz.png",
		"textures/nz.png");

	skybox->OnCreate();

	light = std::make_shared<LightActor>(camera.get(), LightStyle::DirectionLight, Vec3(0.0f, 5.0f, 1.0f), Vec4(0.85f, 0.6, 0.6f, 0.0f));
	light->OnCreate();

	

	

	CreateLevelLayout();


	/// Register the two balls with the physics and collision systems
	physicsSystem.AddActor(player);

	collisionSystem.AddActor(player);


	cube->GetComponent<MeshComponent>()->PrintVertices();
	cube->GetComponent<MeshComponent>()->GetVertMaxX();

	return true;
}



bool CapstoneSceneDream:: CreateLevelLayout() {
	//Static texture shaders
	Ref<ShaderComponent> CubeShader = assetManager->GetComponent<ShaderComponent>("RefularTextureShader");

	//Right island in the middle middle part
	LeftIsland = std::make_shared<Actor>(nullptr);
	LeftIsland->AddComponent<PhysicsComponent>(nullptr, Vec3(3.0f, 1.0f, 0.0f), ///pos
		QMath::angleAxisRotation(45.0f, Vec3(0.0f, 1.0f, 0.0f)), Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	LeftIsland->GetComponent<PhysicsComponent>()->SetScale(Vec3(1.0f, 1.0f, 1.0f));
	LeftIsland->GetComponent<PhysicsComponent>()->isStatic = true;
	Plane colliderPlane = CalculatePlaneCollider(LeftIsland);

	LeftIsland->AddComponent<CollisionComponent>(nullptr, colliderPlane);
	LeftIsland->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Cube"));
	LeftIsland->AddComponent<ShaderComponent>(CubeShader);
	LeftIsland->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("DefaultTexture"));
	AddOpaqueActor(LeftIsland);
	physicsSystem.AddActor(LeftIsland);
	collisionSystem.AddActor(LeftIsland);


	/*MiddleIsland = std::make_shared<Actor>(nullptr);
	MiddleIsland->AddComponent<PhysicsComponent>(nullptr, Vec3(6.0f, -1.0f, 2.0f), ///pos
		QMath::angleAxisRotation(0.0f, Vec3(1.0f, 0.0f, 0.0f)), Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	MiddleIsland->GetComponent<PhysicsComponent>()->SetScale(Vec3(6.0f, 0.5f, 6.0f));
	MiddleIsland->GetComponent<PhysicsComponent>()->isStatic = true;
	MiddleIsland->AddComponent<CollisionComponent>(nullptr, 1.0f);
	MiddleIsland->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Cube"));
	MiddleIsland->AddComponent<ShaderComponent>(CubeShader);
	MiddleIsland->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("DefaultTexture"));
	AddOpaqueActor(MiddleIsland);
	physicsSystem.AddActor(MiddleIsland);
	collisionSystem.AddActor(MiddleIsland);



	House = std::make_shared<Actor>(nullptr);
	House->AddComponent<PhysicsComponent>(nullptr, Vec3(3.0f, 1.0f, -2.0f), ///pos
		QMath::angleAxisRotation(0.0f, Vec3(1.0f, 0.0f, 0.0f)), Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	House->GetComponent<PhysicsComponent>()->SetScale(Vec3(1.0f, 2.5f, 1.0f));
	House->GetComponent<PhysicsComponent>()->isStatic = true;
	House->AddComponent<CollisionComponent>(nullptr, 0.5f);
	House->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Cube"));
	House->AddComponent<ShaderComponent>(CubeShader);
	House->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("DefaultTexture"));
	AddOpaqueActor(House);
	physicsSystem.AddActor(House);
	collisionSystem.AddActor(House);



	RightIsland = std::make_shared<Actor>(nullptr);
	RightIsland->AddComponent<PhysicsComponent>(nullptr, Vec3(15.0f, -3.0f, -4.0f), ///pos
		QMath::angleAxisRotation(0.0f, Vec3(1.0f, 0.0f, 0.0f)), Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	RightIsland->GetComponent<PhysicsComponent>()->SetScale(Vec3(3.0f, 1.0f, 2.0f));
	RightIsland->GetComponent<PhysicsComponent>()->isStatic = true;
	RightIsland->AddComponent<CollisionComponent>(nullptr, 1.0f);
	RightIsland->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Cube"));
	RightIsland->AddComponent<ShaderComponent>(CubeShader);
	RightIsland->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("DefaultTexture"));
	AddOpaqueActor(RightIsland);
	physicsSystem.AddActor(RightIsland);
	collisionSystem.AddActor(RightIsland);*/



	return true;
}


CapstoneSceneDream::~CapstoneSceneDream() {
	Debug::Info("Deleted Scene Dream: ", __FILE__, __LINE__);

	OnDestroy();
}

void CapstoneSceneDream::OnDestroy() {
	Debug::Info("Deleting Scene Dream: ", __FILE__, __LINE__);
}

void CapstoneSceneDream::HandleEvents(const SDL_Event& sdlEvent) {
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


void CapstoneSceneDream::Update(const float deltaTime) {
	
	
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
	/*player->GetComponent<PhysicsComponent>()->Update(deltaTime);
	cube->GetComponent<PhysicsComponent>()->Update(deltaTime);*/


	}


void CapstoneSceneDream::Render() const {
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	

	glBindBuffer(GL_UNIFORM_BUFFER, camera->GetMatriciesID());
	glBindBuffer(GL_UNIFORM_BUFFER, light->GetLightID());

	Ref<ShaderComponent> skyboxShader = skybox->GetComponent<ShaderComponent>();

	glUseProgram(skyboxShader->GetProgram());
	glUniformMatrix4fv(skyboxShader->GetUniformID("modelMatrix"), 1, GL_FALSE, skybox->GetModelMatrix());
	glUniformMatrix4fv(skyboxShader->GetUniformID("viewMatrix"), 1, GL_FALSE, MMath::inverse(camera->orient));

	//camera->orient.print("Camera orientation");
	glUniformMatrix4fv(skyboxShader->GetUniformID("projectionMatrix"), 1, GL_FALSE, camera->GetProjectionMatrix());
	std::dynamic_pointer_cast<SkyBox>(skybox)->Render();
	glUseProgram(0);

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


	//if (drawOverlay == true) {
	//	DrawMeshOverlay(Vec4(1.0f, 1.0f, 1.0f, 0.5f));
	//}

	//if (drawNormals == true) {
	//	DrawNormals(Vec4(1.0f, 1.0f, 0.0f, 0.05f));
	//}
}




void CapstoneSceneDream::DrawUI_imgui()
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


	//Inventory 
	ImGui::SetNextWindowPos(ImVec2(200, 750)); // Set the window position here (x, y)
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





	if (ImGui::Button("Toggle Inventory Window")) {
		inventoryButtonPressed = !inventoryButtonPressed;
	}

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


		
;

		// Or here?

		//ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));

		//ImGui::Text("Hello, world!");
		//ImGui::PopStyleColor();
		//  
		////regular button
		//ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));  // Red Button
		//ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.0f, 0.0f, 1.0f)); // Darker red when hovered
		//ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.0f, 0.0f, 1.0f));  // Even darker when clicked

		//ImGui::SetCursorPos(ImVec2(100, 100));
		//if (ImGui::Button("Click Me")) {
		//	printf("ButtonCLicked");
		//}
		//ImGui::PopStyleColor(3);

		//image button
	
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


Plane CapstoneSceneDream::CalculatePlaneCollider(Ref<Actor> obj) {
	Ref<PhysicsComponent> objTransform = obj->GetComponent<PhysicsComponent>();
	Vec3 pos = objTransform->GetPosition();
	Vec3 scale = objTransform->GetScale();

	// Assuming your cube mesh is centered at its origin and has a size of 1
	// The top surface would be at half the height of the cube
	float halfHeight = scale.y * 0.5f;

	// Calculate position of the top surface
	Vec3 topSurfacePos = pos;
	topSurfacePos.y += halfHeight; // Move up by half height

	// Normal points upward for top surface
	Vec3 normal = Vec3(0.0f, 1.0f, 0.0f);
	Quaternion rotation = objTransform->GetQuaternion();
	Quaternion identity =  Quaternion(1.0f, Vec3(0.0f, 0.0f, 0.0f));
	// If the object has rotation, apply it to the normal
	
	if (abs(QMath::dot(rotation, identity)) < 0.99999f) {
		normal = VMath::normalize(QMath::rotate(normal, rotation ));
	}

	// Calculate the distance from origin to plane
	// For a plane equation ax + by + cz + d = 0
	// d = -dot(normal, point_on_plane)
	float distance = - VMath::dot(normal, topSurfacePos);

	return Plane(normal, distance);
}

















void CapstoneSceneDream::DrawNormals(const Vec4 color) const {
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



void CapstoneSceneDream::DrawMeshOverlay(const Vec4 color) const {
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

