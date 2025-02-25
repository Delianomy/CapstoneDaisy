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


CapstoneSceneDream::CapstoneSceneDream() :drawNormals(false), drawOverlay(false) {
	Debug::Info("Created Scene Dream: ", __FILE__, __LINE__);
	glGetIntegerv(GL_VIEWPORT, viewport);
}

bool CapstoneSceneDream::OnCreate() {
	Debug::Info("Loading assets Scene Dream: ", __FILE__, __LINE__);
	assetManager = std::make_shared<AssetManager>();

	Ref<ShaderComponent> shader = assetManager->GetComponent<ShaderComponent>("TextureShader");
	Ref<ShaderComponent> CubeShader = assetManager->GetComponent<ShaderComponent>("RefularTexture");
	Ref<ShaderComponent> WaveShader = assetManager->GetComponent<ShaderComponent>("WaveShader");


	//make an actor
	player = std::make_shared<Actor>(nullptr);
	Quaternion orientation = QMath::angleAxisRotation(0.0f, Vec3(1.0f, 0.0f, 0.0f));

	//add transform (Maybe physics component now substitudes the transfrom)
	//player->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, 0.0f), orientation);
	//add physcis component
	player->AddComponent<PhysicsComponent>(nullptr, Vec3(0.0f, 0.0f, 0.0f),/// pos
		QMath::angleAxisRotation(0.0f, Vec3(1.0f, 0.0f, 0.0f)),
		Vec3(0.0f, 0.0f, 0.0f) ///velocity
		);
	player->GetComponent<PhysicsComponent>()->SetScale(Vec3(2.0f, 2.0f, 2.0f));
	/// This makes a Sphere Collision Component because of the argument list - just the radius. 
	player->AddComponent<CollisionComponent>(nullptr, 1.0f);
	player->GetComponent<PhysicsComponent>()->isStatic = false;
	player->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Square"));
	player->AddComponent<ShaderComponent>(shader);
	player->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Sprite_Sheet_HEAVY"));

	AddActor(player);

	//now technically it would mean that out player now has a collider
	GLint maxTextureSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
	printf("Max Texture Size: %d\n", maxTextureSize);

	GLint totalMemoryKB;
	glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &totalMemoryKB);
	printf("Available GPU Memory: %d KB\n", totalMemoryKB);

	camera = std::make_shared<CameraActor>(player.get());
	camera->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, -3.0f), Quaternion());
	camera->OnCreate();
	camera->GetProjectionMatrix().print("ProjectionMatrix");
	camera->GetViewMatrix().print("ViewMatrix");



	skybox = std::make_shared<SkyBox>(nullptr, "textures/px.png", "textures/nx.png",
		"textures/py.png", "textures/ny.png", "textures/pz.png",
		"textures/nz.png");

	skybox->OnCreate();

	light = std::make_shared<LightActor>(camera.get(), LightStyle::DirectionLight, Vec3(0.0f, 5.0f, 1.0f), Vec4(0.85f, 0.6, 0.6f, 0.0f));
	light->OnCreate();


	
	//cube = std::make_shared<Actor>(nullptr);
	//cube->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, -3.0f), orientation);
	//cube->GetComponent<TransformComponent>()->SetScale(Vec3(0.1f, 0.1f, 0.1f));
	//cube->AddComponent<PhysicsComponent>(nullptr, Vec3(0.0f, 0.0f, -4.0f),/// pos
	//	QMath::angleAxisRotation(0.0f, Vec3(1.0f, 0.0f, 0.0f)), Vec3(0.0f, 0.0f, 0.0f));
	//cube->GetComponent<PhysicsComponent>()->SetScale(Vec3(5.0f, 5.0f, 5.0f));
	//cube->AddComponent<CollisionComponent>(nullptr, 1.0f);
	//cube->GetComponent<PhysicsComponent>()->isStatic = true;
	//cube->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Cube"));
	//cube->AddComponent<ShaderComponent>(WaveShader);
	//cube->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Sprite_Sheet_HEAVY"));
	//AddActor(cube);


	/// Register the two balls with the physics and collision systems
	physicsSystem.AddActor(player);
	//physicsSystem.AddActor(cube);
	collisionSystem.AddActor(player);
	//collisionSystem.AddActor(cube);

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
	ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
}

void CapstoneSceneDream::Update(const float deltaTime) {
	
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("name");
	ImGui::SetCursorPos(ImVec2(100, 100));
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
	
	ImGui::Text("Hello, world!");
	ImGui::PopStyleColor();


	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));  // Red Button
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.0f, 0.0f, 1.0f)); // Darker red when hovered
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.0f, 0.0f, 1.0f));  // Even darker when clicked

	ImGui::SetCursorPos(ImVec2(100, 100));
	if (ImGui::Button("Click Me")) {
		printf("ButtonCLicked");
	}
	ImGui::PopStyleColor(3);

	ImGui::End();
	

	iTime += deltaTime;
	player->GetComponent<PhysicsComponent>()->SetVel(Vec3(0.0, 0.0, 0.0));

	if (goRight) {
		currentTime += deltaTime;
		animIndex = static_cast<int>(currentTime / frameSpeed) % 13;

		Ref<PhysicsComponent> physics = player->GetComponent<PhysicsComponent>();
		physics->SetVel(Vec3(1.0, 0.0, 0.0));

		/*Vec3 currentPos = physics->GetPosition();
		Quaternion currentRot = physics->GetQuaternion();

		physics->SetPosition(currentPos + MMath::toMatrix4(currentRot) * lefr_right_Vector);*/

	}
	if (goLeft) {
		currentTime += deltaTime;
		animIndex = static_cast<int>(currentTime / frameSpeed) % 13;

		Ref<PhysicsComponent> physics = player->GetComponent<PhysicsComponent>();
		physics->SetVel(Vec3(-1.0, 0.0, 0.0));

		/*Vec3 currentPos = physics->GetPosition();
		Quaternion currentRot = physics->GetQuaternion();

		physics->SetPosition(currentPos - MMath::toMatrix4(currentRot) * lefr_right_Vector);*/

	}
	if (goForward) {
		currentTime += deltaTime;
		animIndex = static_cast<int>(currentTime / frameSpeed) % 13;

		Ref<PhysicsComponent> physics = player->GetComponent<PhysicsComponent>();
		physics->SetVel(Vec3(0.0, 0.0, 1.0));
		/*Vec3 currentPos = physics->GetPosition();
		Quaternion currentRot = physics->GetQuaternion();

		physics->SetPosition(currentPos + MMath::toMatrix4(currentRot) * forwardVector);*/

	}

	if (goBackwards) {
		currentTime += deltaTime;
		animIndex = static_cast<int>(currentTime / frameSpeed) % 13;

		Ref<PhysicsComponent> physics = player->GetComponent<PhysicsComponent>();
		physics->SetVel(Vec3(0.0, 0.0, -1.0));
		/*Vec3 currentPos = physics->GetPosition();
		Quaternion currentRot = physics->GetQuaternion();

		physics->SetPosition(currentPos - MMath::toMatrix4(currentRot) * forwardVector);*/

	}

		if (rotatePlayerLeft) {

			playerAngle += 0.4f;
			Quaternion newQuat = QMath::angleAxisRotation(playerAngle, Vec3(0.0f, 1.0f, 0.0f));


			player->GetComponent<PhysicsComponent>()->SetQuaternion(newQuat);

			//camera->GetViewMatrix().print("ViewMatrix");
		}

		if (rotatePlayerRight) {

			playerAngle -= 0.4f;
			Quaternion newQuat = QMath::angleAxisRotation(playerAngle, Vec3(0.0f, 1.0f, 0.0f));


			player->GetComponent<PhysicsComponent>()->SetQuaternion(newQuat);

			//camera->GetViewMatrix().print("ViewMatrix");
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


	for (auto actor : actors) {
		glUseProgram(actor->GetComponent<ShaderComponent>()->GetProgram());
		glUniform1f(actor->GetComponent<ShaderComponent>()->GetUniformID("index"), animIndex);
		glUniformMatrix4fv(actor->GetComponent<ShaderComponent>()->GetUniformID("modelMatrix"), 1, GL_FALSE, actor->GetModelMatrix());
		glBindTexture(GL_TEXTURE_2D, actor->GetComponent<MaterialComponent>()->getTextureID());
		actor->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
	}
	
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	if (drawOverlay == true) {
		DrawMeshOverlay(Vec4(1.0f, 1.0f, 1.0f, 0.5f));
	}

	if (drawNormals == true) {
		DrawNormals(Vec4(1.0f, 1.0f, 0.0f, 0.05f));
	}
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


