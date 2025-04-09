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
#include "TriggerComponent.h"
#include <random>
#include <sstream>

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
	interactionManager = std::make_shared<InteractionManager>();

	InitializeDialogue();
	//SDL_SetCursor(SDL_GetDefaultCursor());

	currentAnim = PlayerAnimType::Idle;
	dialogueSystem = std::make_shared<DialogueSystem>();
	audioManager = std::make_shared<AudioManager>();
	dialogueSystem->SetAudioManager(audioManager);


	//Shaders used in the scene
	Ref<ShaderComponent> shader = assetManager->GetComponent<ShaderComponent>("TextureShader");
	Ref<ShaderComponent> WaveShader = assetManager->GetComponent<ShaderComponent>("WaveShader");
	Ref<ShaderComponent> CubeShader = assetManager->GetComponent<ShaderComponent>("RegularTextureShader");
	Ref<ShaderComponent> WaterShader = assetManager->GetComponent<ShaderComponent>("Water Shader");

	CreateDebugMeshes();


	Ocean = std::make_shared<Actor>(nullptr);
	Ocean->AddComponent<PhysicsComponent>(nullptr, Vec3(0.0f, -13.0f, 0.0f),/// pos
		QMath::angleAxisRotation(90.0f, Vec3(0.0f, 1.0f, 0.0f)),
		Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	Ocean->GetComponent<PhysicsComponent>()->SetScale(Vec3(20.0f, 10.0f, 20.0f));
	Ocean->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Sphere"));
	Ocean->AddComponent<ShaderComponent>(WaterShader);
	Ocean->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Water_normal"));
	AABB oceanCollider = AABB(Vec3(0.0f, -3.0f, 0.0f), Vec3(20.0f, 5.0f, 20.0f));
	Ocean->AddComponent<TriggerComponent>(nullptr, oceanCollider);
	Ocean->GetComponent<TriggerComponent>()->SetCallback(TriggerCallbackCreator::CreateTriggerCallback(this, &CapstoneSceneDream::OnEnterOcean));
	triggerSystem.AddActor(Ocean);



	//Ref<AssetManager> assMan, Vec3 pos, float triggerRadius = 0.5f, Vec3 scale = Vec3(0.1f, 0.1f, 0.1f), std::shared_ptr<MaterialComponent> material = nullptr

		//make an actor
	player = std::make_shared<Actor>(nullptr);
	player->NPCid = 0;
	player->AddComponent<PhysicsComponent>(nullptr, Vec3(0.0f, 0.0f, 0.0f),/// pos
		QMath::angleAxisRotation(0.0f, Vec3(1.0f, 0.0f, 0.0f)),
		Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	player->GetComponent<PhysicsComponent>()->SetScale(Vec3(1.0f, 1.0f, 1.0f));
	/// This makes a Sphere Collision Component because of the argument list - just the radius. 
	player->AddComponent<CollisionComponent>(nullptr, 0.5f);
	player->GetComponent<PhysicsComponent>()->isStatic = false;
	player->GetComponent<PhysicsComponent>()->useGravity = false;
	player->GetComponent<PhysicsComponent>()->mass = 1.0f;
	player->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Square"));
	player->AddComponent<ShaderComponent>(shader);
	player->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Daisy_spriteSheet"));
	player->AddComponent<TriggerComponent>(nullptr, 1.0f);

	player->GetComponent<TriggerComponent>()->SetCallback(this, &CapstoneSceneDream::PlayerTriggerCallback);
	AddTransparentActor(player);


	mermaid = std::make_shared<InteractableActor>(assetManager, Vec3(0.0f, 0.0f, 0.0f), 1.5f, Vec3(0.5f, 0.5f, 0.5f), assetManager->GetComponent<MaterialComponent>("Mermaid"), assetManager->GetComponent<ShaderComponent>("TextureShader"));
	mermaid->NPCid = 1;
	mermaid->Bind([this]() {
		int index = mermaid->NPCid;
	/*	inventoryButtonPressed = !inventoryButtonPressed;*/
		if (index >= 0 && index < dialogueSequences.size()) {
			dialogueSystem->ClearDialogues();

			for (const auto& dialogue : dialogueSequences[index]) {
				dialogueSystem->AddDialogueToSequence(dialogue);
			}

			dialogueSystem->OpenDialogue(0);
		}
		std::cout << "Interacted with mermaid";
		});
	mermaid->AddComponent<CollisionComponent>(nullptr, 0.5f);
	triggerSystem.AddActor(mermaid);
	AddTransparentActor(mermaid);


	mrOwl = std::make_shared<InteractableActor>(assetManager, Vec3(11.0f, -0.2f, 1.0f), 1.5f, Vec3(0.2f, 0.2f, 0.2f), assetManager->GetComponent<MaterialComponent>("Owl"), assetManager->GetComponent<ShaderComponent>("TextureShader"));
	mrOwl->NPCid = 2;
	mrOwl->Bind([this]() {
	/*	inventoryButtonPressed = !inventoryButtonPressed;*/

		std::cout << "Interacted with owl";
		});
	mrOwl->AddComponent<CollisionComponent>(nullptr, 0.5f);
	triggerSystem.AddActor(mrOwl);
	AddTransparentActor(mrOwl);





	mrsMouse = std::make_shared<InteractableActor>(assetManager, Vec3(8.7f, -0.18f, -0.05f), 1.0f, Vec3(0.1f, 0.1f, 0.1f), assetManager->GetComponent<MaterialComponent>("Mouse"), assetManager->GetComponent<ShaderComponent>("TextureShader"));
	mrsMouse->NPCid = 3;
	mrsMouse->Bind([this]() {
		/*	inventoryButtonPressed = !inventoryButtonPressed;*/

		std::cout << "Interacted with owl";
		});
	mrsMouse->AddComponent<CollisionComponent>(nullptr, 0.5f);
	triggerSystem.AddActor(mrsMouse);
	AddTransparentActor(mrsMouse);




	fairy = std::make_shared<InteractableActor>(assetManager, Vec3(2.0f, 0.0f, 0.0f), 1.0f, Vec3(0.1f, 0.1f, 0.1f), assetManager->GetComponent<MaterialComponent>("Fairy"), assetManager->GetComponent<ShaderComponent>("TextureShader"));
	fairy->NPCid = 4;
	fairy->Bind([this]() {
		/*	inventoryButtonPressed = !inventoryButtonPressed;*/

		std::cout << "Interacted with owl";
		});
	fairy->AddComponent<CollisionComponent>(nullptr, 0.5f);
	triggerSystem.AddActor(fairy);
	AddTransparentActor(fairy);



	//now technically it would mean that out player now has a collider
	GLint maxTextureSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
	printf("Max Texture Size: %d\n", maxTextureSize);

	GLint totalMemoryKB;
	glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &totalMemoryKB);
	printf("Available GPU Memory: %d KB\n", totalMemoryKB);



	CreateLevelLayout();

	

	camera = std::make_shared<CameraActor>(player.get());
	camera->isInMainMenu = false;
	camera->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, -2.0f), Quaternion());
	camera->OnCreate();
	camera->GetProjectionMatrix().print("ProjectionMatrix");
	camera->GetViewMatrix().print("ViewMatrix");
	AddActor(camera);

	skybox = std::make_shared<SkyBox>(nullptr, "textures/Skyboxes/Overworld/px.png", "textures/Skyboxes/Overworld/nx.png",
		"textures/Skyboxes/Overworld/py.png", "textures/Skyboxes/Overworld/ny.png", "textures/Skyboxes/Overworld/pz.png",
		"textures/Skyboxes/Overworld/nz.png");

	skybox->OnCreate();

	light = std::make_shared<LightActor>(camera.get(), LightStyle::DirectionLight, Vec3(0.0f, 5.0f, 1.0f), Vec4(0.85f, 0.6, 0.6f, 0.0f));
	light->OnCreate();
	/// Register the two balls with the physics and collision systems
	physicsSystem.AddActor(player);
	collisionSystem.AddActor(player);
	triggerSystem.AddActor(player);


	////FrameBuffer
	//glGenFramebuffers(1, &frameBuffer);
	//glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);


	//glGenTextures(1, &textureColorbuffer);
	//glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	////RESET THE SCREEN WIDTH AND HEIGHT LATER!!!
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1366, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);


	//glGenRenderbuffers(1, &rbo);
	//glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1366, 768);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	//
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//float triangleVertices[] = {
	//	-1.0f, -1.0f,     0.0f, 0.0f,  // Bottom-left
	//	 3.0f, -1.0f,     1.0f, 0.0f,  // Bottom-right (going beyond the NDC range for full screen)
	//	-1.0f,  3.0f,     0.0f, 1.0f   // Top-left (going beyond the NDC range for full screen)
	//};

	//glGenVertexArrays(1, &VAO);
	//glGenBuffers(1, &VBO);

	//// Bind the VAO
	//glBindVertexArray(VAO);

	//// Bind and populate the VBO
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

	//// Define the position attribute (first 2 components of each vertex)
	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

	//// Define the texture coordinate attribute (last 2 components of each vertex)
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 *sizeof(float), (void*)(2 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//// Unbind VAO
	//glBindVertexArray(0);

	return true;
}

CapstoneSceneDream::~CapstoneSceneDream() {
	Debug::Info("Deleted Scene Dream: ", __FILE__, __LINE__);

	OnDestroy();
}

void CapstoneSceneDream::OnDestroy() {
	Debug::Info("Deleting Scene Dream: ", __FILE__, __LINE__);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteFramebuffers(1, &rbo);
	glDeleteFramebuffers(1, &frameBuffer);
	glDeleteTextures(1, &textureColorbuffer);
}

void CapstoneSceneDream::HandleEvents(const SDL_Event& sdlEvent) {

	static int objID = -1;
	static Vec2 currentMousePos;
	static Vec2	lastMousePos;
	static float flip = 1.0f;
	Ref<TransformComponent> cameraTC;
	Ref<TransformComponent> gameBoardTC;
	float cameraMoveSpeed = 0.1f;
	Vec3 cameraNewPos;
	Ref<InteractableActor> currentInteraction;
	/// Handle Camera movement 
	switch (sdlEvent.type) {
	case SDL_KEYDOWN:
		if (!anyInputPressed) {
			player->GetComponent<PhysicsComponent>()->useGravity = true;
			anyInputPressed = true;
		}
		cameraTC = camera->GetComponent<TransformComponent>();
		switch (sdlEvent.key.keysym.scancode) {

		case SDL_SCANCODE_SPACE:
			if (playerIsGrounded || underwater) {
				movementInput.y = 1.0f;
			}
			break;
		case SDL_SCANCODE_LCTRL:
			if (underwater) {
				movementInput.y = -1.0f;
			}
			break;

		case SDL_SCANCODE_Q:
			cameraTC->SetTransform(cameraTC->GetPosition(), cameraTC->GetQuaternion() *
				QMath::angleAxisRotation(2.0f, Vec3(0.0f, 1.0f, 0.0f)));
			break;
		case SDL_SCANCODE_E:
			inventoryButtonPressed = !inventoryButtonPressed;
			if (inventoryButtonPressed) {
				audioManager->Play(0, 0.5f);
			}
			else {
				audioManager->Play(1, 0.1f);
			}

			//Pend the item to the inventory
			//The function already checks if it's nullptr so don't worry about it
			PendItemToInventory(interactionManager->GetCurrentInteraction());
			break;
		case SDL_SCANCODE_1:
			if (inventoryButtonPressed) {
				std::shared_ptr<ItemInteractable> itemInteraction = std::dynamic_pointer_cast<ItemInteractable>(interactionManager->GetCurrentInteraction());
				if (itemInteraction != nullptr) {
					itemInteraction->TryItem(inventory->items[0]);
					break;
				}

				AddItemToInventory(inventory->pendingItem, 0);
			}
			break;

		case SDL_SCANCODE_2:
			if (inventoryButtonPressed) {
				std::shared_ptr<ItemInteractable> itemInteraction = std::dynamic_pointer_cast<ItemInteractable>(interactionManager->GetCurrentInteraction());
				if (itemInteraction != nullptr) {
					itemInteraction->TryItem(inventory->items[1]);
					break;
				}

				AddItemToInventory(inventory->pendingItem, 1);
			}
			break;

		case SDL_SCANCODE_3:
			if (inventoryButtonPressed) {
				std::shared_ptr<ItemInteractable> itemInteraction = std::dynamic_pointer_cast<ItemInteractable>(interactionManager->GetCurrentInteraction());
				if (itemInteraction != nullptr) {
					itemInteraction->TryItem(inventory->items[2]);
					break;
				}

				AddItemToInventory(inventory->pendingItem, 2);
			}
			break;

		case SDL_SCANCODE_F:
			//Add the current interactable actor into the interactionManager
			currentInteraction = interactionManager->GetCurrentInteraction();
			if (currentInteraction == nullptr) { break; }

			//Check if it's a pickable item
			//Skip if that's the case
			if (std::dynamic_pointer_cast<PickableItem>(currentInteraction) != nullptr) { break; }

			//Calls the callback function
			currentInteraction->Invoke();
			break;

		case SDL_SCANCODE_A:
			movementInput += Vec3(-1.0, 0.0, 0.0);
			break;

		case SDL_SCANCODE_W:
			movementInput += Vec3(0.0, 0.0, -1.0);
			break;

		case SDL_SCANCODE_D:
			movementInput += Vec3(1.0, 0.0, 0.0);
			break;

		case SDL_SCANCODE_S:
			movementInput += Vec3(0.0, 0.0, 1.0);
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

		case SDL_SCANCODE_SPACE:
			movementInput.y = 0.0f;
			break;

		case SDL_SCANCODE_LCTRL:
			movementInput.y = 0.0f;
			break;

		case SDL_SCANCODE_A:
			movementInput.x = 0.0f;
			break;
		case SDL_SCANCODE_W:
			movementInput.z = 0.0f;
			break;
		case SDL_SCANCODE_D:
			movementInput.x = 0.0f;
			break;
		case SDL_SCANCODE_S:
			movementInput.z = 0.0f;
			break;
		default:
			break;

		}
	case SDL_MOUSEBUTTONDOWN:
		if (ImGui::GetIO().WantCaptureMouse) break;

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

	case SDL_MOUSEWHEEL:{
		cameraTC = camera->GetComponent<TransformComponent>();
		if (sdlEvent.wheel.y > 0.0f) {
			cameraNewPos = cameraTC->GetPosition() + Vec3(0.0, 0.0, cameraMoveSpeed);
			cameraTC->SetPosition(cameraNewPos);
			break;
		}

		cameraNewPos = cameraTC->GetPosition() - Vec3(0.0, 0.0, cameraMoveSpeed);
		cameraTC->SetPosition(cameraNewPos);
		break;
	}

	ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
	}
}

bool CapstoneSceneDream::CreateLevelLayout() {
	Ref<ShaderComponent> CubeShader = assetManager->GetComponent<ShaderComponent>("RegularTextureShader");
	Ref<ShaderComponent> shader = assetManager->GetComponent<ShaderComponent>("TextureShader");


	Ref<Actor> bush1 = std::make_shared<Actor>(nullptr);
	bush1->AddComponent<PhysicsComponent>(nullptr, Vec3(10.0f, 0.0f, 1.0f),/// pos
		QMath::angleAxisRotation(0, Vec3(0.0, 1.0f, 0.0f)),
			Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	bush1->GetComponent<PhysicsComponent>()->SetScale(Vec3(1.0f, 1.0f, 1.0f));
	bush1->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Bush_obj"));
	bush1->GetComponent<PhysicsComponent>()->isStatic = true;

	bush1->AddComponent<ShaderComponent>(shader);
	bush1->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Bush_mat"));
	bush1->AddComponent<TriggerComponent>(nullptr, 1.0f);
	AddTransparentActor(bush1);


	Ref<Actor> grass = std::make_shared<Actor>(nullptr);
	grass->AddComponent<PhysicsComponent>(nullptr, Vec3(10.0f, -1.0f, 1.0f),/// pos
		QMath::angleAxisRotation(0, Vec3(1.0, 0.0f, 0.0f)),
		Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	grass->GetComponent<PhysicsComponent>()->SetScale(Vec3(1.0f, 1.0f, 1.0f));
	grass->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Grass1_obj"));
	grass->AddComponent<ShaderComponent>(shader);
	grass->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Grass1_mat"));
	grass->AddComponent<TriggerComponent>(nullptr, 1.0f);
	AddTransparentActor(grass);


	//island in the centre
	Ref<Actor>Island1 = std::make_shared<Actor>(nullptr);
	Island1->tag = GROUND;
	Island1->AddComponent<PhysicsComponent>(nullptr, Vec3(0.0f, -1.5f, 0.0f),/// pos
		QMath::angleAxisRotation(90.0f, Vec3(0.0f, 1.0f, 0.0f)),
		Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	Island1->GetComponent<PhysicsComponent>()->SetScale(Vec3(1.0f, 1.0f, 1.0f));
	Island1->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Island1_obj"));
	AABB cubeCollider;
	cubeCollider.center = Island1->GetComponent<PhysicsComponent>()->GetPosition();
	cubeCollider.rx = 3.18f;
	cubeCollider.ry = 0.51f;
	cubeCollider.rz = 2.8f;
	Island1->AddComponent<CollisionComponent>(nullptr, cubeCollider);
	Island1->GetComponent<PhysicsComponent>()->isStatic = true;
	Island1->AddComponent<ShaderComponent>(CubeShader);
	Island1->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Island1_mat"));
	AddOpaqueActor(Island1);
	physicsSystem.AddActor(Island1);
	collisionSystem.AddActor(Island1);


	//island with the house
	Ref<Actor>Island2 = std::make_shared<Actor>(nullptr);
	Island2->tag = GROUND;
	Island2->AddComponent<PhysicsComponent>(nullptr, Vec3(11.0f, -1.9f, 0.0f),/// pos
		QMath::angleAxisRotation(0.0f, Vec3(1.0f, 0.0f, 0.0f)),
		Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	Island2->GetComponent<PhysicsComponent>()->SetScale(Vec3(1.0f, 0.76f, 1.0f));
	Island2->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Island2_obj"));
	Island2->GetComponent<PhysicsComponent>()->isStatic = true;
	Island2->AddComponent<ShaderComponent>(CubeShader);
	Island2->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Island2_mat"));
	Island2->AddComponent<CollisionComponent>(nullptr, 1.0f);
	AddOpaqueActor(Island2);
	physicsSystem.AddActor(Island2);
	collisionSystem.AddActor(Island2);



	////Small island
	Ref<Actor>Island3 = std::make_shared<Actor>(nullptr);
	Island3->tag = GROUND;
	Island3->AddComponent<PhysicsComponent>(nullptr, Vec3(2.5f, -1.9f, 4.2f),/// pos
		QMath::angleAxisRotation(0.0f, Vec3(1.0f, 0.0f, 0.0f)),
		Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	cubeCollider;
	cubeCollider.center = Island3->GetComponent<PhysicsComponent>()->GetPosition();
	cubeCollider.rx = 0.5f;
	cubeCollider.ry = 0.3f;
	cubeCollider.rz = 0.5f;

	Island3->GetComponent<PhysicsComponent>()->SetScale(Vec3(0.1f, 0.1f, 0.1f));
	Island3->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Island2_obj"));
	Island3->GetComponent<PhysicsComponent>()->isStatic = true;
	Island3->AddComponent<ShaderComponent>(CubeShader);
	Island3->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Island2_mat"));
	Island3->AddComponent<CollisionComponent>(nullptr, cubeCollider);
	AddOpaqueActor(Island3);
	physicsSystem.AddActor(Island3);
	collisionSystem.AddActor(Island3);



	////Small island 2
	Ref<Actor>Island4 = std::make_shared<Actor>(nullptr);
	Island4->tag = GROUND;
	Island4->AddComponent<PhysicsComponent>(nullptr, Vec3(5.3f, -2.4f, 2.0f),/// pos
		QMath::angleAxisRotation(0.0f, Vec3(1.0f, 0.0f, 0.0f)),
		Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	cubeCollider.center = Island4->GetComponent<PhysicsComponent>()->GetPosition();
	cubeCollider.rx = 0.55f;
	cubeCollider.ry = 0.25f;
	cubeCollider.rz = 0.55f;
	Island4->GetComponent<PhysicsComponent>()->SetScale(Vec3(0.2f, 0.2f, 0.2f));
	Island4->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Island2_obj"));
	Island4->GetComponent<PhysicsComponent>()->isStatic = true;
	Island4->AddComponent<ShaderComponent>(CubeShader);
	Island4->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Island2_mat"));
	Island4->AddComponent<CollisionComponent>(nullptr, cubeCollider);
	AddOpaqueActor(Island4);
	physicsSystem.AddActor(Island4);
	collisionSystem.AddActor(Island4);



	////Small island 3
	//Ref<Actor>Island5 = std::make_shared<Actor>(nullptr);
	//Island5->AddComponent<PhysicsComponent>(nullptr, Vec3(6.8f, -2.0f, 0.0f),/// pos
	//	QMath::angleAxisRotation(0.0f, Vec3(1.0f, 0.0f, 0.0f)),
	//	Vec3(0.0f, 0.0f, 0.0f) ///velocity
	//);
	//Island5->GetComponent<PhysicsComponent>()->SetScale(Vec3(0.2f, 0.2f, 0.2f));
	//Island5->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Island2_obj"));
	//Island5->GetComponent<PhysicsComponent>()->isStatic = true;
	//Island5->AddComponent<ShaderComponent>(CubeShader);
	//Island5->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Island2_mat"));
	//Island5->AddComponent<CollisionComponent>(nullptr, 0.5f);
	//AddOpaqueActor(Island5);
	//physicsSystem.AddActor(Island5);
	//collisionSystem.AddActor(Island5);



	Ref<Actor>BottomOfTheOcean = std::make_shared<Actor>(nullptr);
	BottomOfTheOcean->AddComponent<PhysicsComponent>(nullptr, Vec3(18.0f, -30.0f, 0.0f),/// pos
		QMath::angleAxisRotation(0.0f, Vec3(1.0f, 0.0f, 0.0f)),
		Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	BottomOfTheOcean->GetComponent<PhysicsComponent>()->SetScale(Vec3(10.0f, 0.1f, 10.0f));
	BottomOfTheOcean->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Island2_obj"));
	cubeCollider;
	cubeCollider.center = BottomOfTheOcean->GetComponent<PhysicsComponent>()->GetPosition();
	//Problem, this looks a bit weird cause Y goes very deep in the bottom comparing to the top side, leadingfor stuff to look sketchy 
	cubeCollider.rx = 3.18f;
	cubeCollider.ry = 0.51f;
	cubeCollider.rz = 0.904f;
	//cube->AddComponent<CollisionComponent>(nullptr, cubeCollider);
	BottomOfTheOcean->GetComponent<PhysicsComponent>()->isStatic = true;
	BottomOfTheOcean->AddComponent<ShaderComponent>(CubeShader);
	BottomOfTheOcean->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Island2_mat"));
	//BottomOfTheOcean->AddComponent<CollisionComponent>(nullptr, 1.0f);
	AddOpaqueActor(BottomOfTheOcean);
	physicsSystem.AddActor(BottomOfTheOcean);
	collisionSystem.AddActor(BottomOfTheOcean);




	Ref<Actor>House = std::make_shared<Actor>(nullptr);
	House->AddComponent<PhysicsComponent>(nullptr, Vec3(8.8f, -0.3f, -0.5f),/// pos
		QMath::angleAxisRotation(0.0f, Vec3(1.0f, 0.0f, 0.0f)),
		Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	House->GetComponent<PhysicsComponent>()->SetScale(Vec3(1.0f, 1.0f, 1.0f));
	House->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("House_obj"));
	cubeCollider.center = House->GetComponent<PhysicsComponent>()->GetPosition();
	//Problem, this looks a bit weird cause Y goes very deep in the bottom comparing to the top side, leadingfor stuff to look sketchy 
	cubeCollider.rx = 0.7f;
	cubeCollider.ry = 1.5f;
	cubeCollider.rz = 0.7f;
	House->AddComponent<CollisionComponent>(nullptr, cubeCollider);
	House->GetComponent<PhysicsComponent>()->isStatic = true;
	House->AddComponent<ShaderComponent>(CubeShader);
	House->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("House_mat"));
	House->AddComponent<TriggerComponent>(nullptr, 1.0f);
	AddOpaqueActor(House);
	physicsSystem.AddActor(House);
	collisionSystem.AddActor(House);




	Ref<Actor>Flower_1 = std::make_shared<Actor>(nullptr);
	Flower_1->AddComponent<PhysicsComponent>(nullptr, Vec3(8.8f, -1.30f, 2.0f),/// pos
		QMath::angleAxisRotation(20.0f, Vec3(-15.0f, 1.0f, -1.0f)),
		Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	Flower_1->GetComponent<PhysicsComponent>()->SetScale(Vec3(0.2f, 0.2f, 0.2f));
	Flower_1->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Flower_obj"));
	Flower_1->GetComponent<PhysicsComponent>()->isStatic = true;

	Flower_1->AddComponent<ShaderComponent>(CubeShader);
	Flower_1->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Flower_v1_mat"));
	Flower_1->AddComponent<TriggerComponent>(nullptr, 1.0f);
	AddOpaqueActor(Flower_1);

	

	



	Ref<Actor>Flower_2 = std::make_shared<Actor>(nullptr);
	Flower_2->AddComponent<PhysicsComponent>(nullptr, Vec3(11.3f, -1.20f, 2.8f),/// pos
		QMath::angleAxisRotation(20.0f, Vec3(1.0f, 1.0f, 1.0f)),
		Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	Flower_2->GetComponent<PhysicsComponent>()->SetScale(Vec3(0.3f, 0.3f, 0.3f));
	Flower_2->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Flower_obj"));
	Flower_2->GetComponent<PhysicsComponent>()->isStatic = true;
	Flower_2->AddComponent<ShaderComponent>(CubeShader);
	Flower_2->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Flower_v1_mat"));
	Flower_2->AddComponent<TriggerComponent>(nullptr, 1.0f);
	AddOpaqueActor(Flower_2);


	Ref<Actor>Flower_3 = std::make_shared<Actor>(nullptr);
	Flower_3->AddComponent<PhysicsComponent>(nullptr, Vec3(12.3f, 0.0f, 1.3f),/// pos
		QMath::angleAxisRotation(50.0f, Vec3(1.0f, 0.0f, 0.0f)),
		Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	Flower_3->GetComponent<PhysicsComponent>()->SetScale(Vec3(0.25f, 0.25f, 0.25f));
	Flower_3->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Flower_obj"));
	Flower_3->GetComponent<PhysicsComponent>()->isStatic = true;
	Flower_3->AddComponent<ShaderComponent>(CubeShader);
	Flower_3->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Flower_v1_mat"));
	Flower_3->AddComponent<TriggerComponent>(nullptr, 1.0f);
	AddOpaqueActor(Flower_3);


	Ref<Actor>Flower_4 = std::make_shared<Actor>(nullptr);
	Flower_4->AddComponent<PhysicsComponent>(nullptr, Vec3(12.3f, -0.25f, -0.9f),/// pos
		QMath::angleAxisRotation(20.0f, Vec3(-1.0f, 1.0f, 0.0f)),
		Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	Flower_4->GetComponent<PhysicsComponent>()->SetScale(Vec3(0.25f, 0.25f, 0.25f));
	Flower_4->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Flower_obj"));
	Flower_4->GetComponent<PhysicsComponent>()->isStatic = true;
	Flower_4->AddComponent<ShaderComponent>(CubeShader);
	Flower_4->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Flower_v1_mat"));
	Flower_4->AddComponent<TriggerComponent>(nullptr, 1.0f);
	AddOpaqueActor(Flower_4);



	/* --------------------ISLAND WITH THE LAKE-------------------- */


	Ref<Actor>Beanstalk1 = std::make_shared<Actor>(nullptr);
	Beanstalk1->AddComponent<PhysicsComponent>(nullptr, Vec3(2.5f, 0.3f, -0.9f),/// pos
		QMath::angleAxisRotation(0.0f, Vec3(-1.0f, 1.0f, 0.0f)),
		Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	cubeCollider.center = Beanstalk1->GetComponent<PhysicsComponent>()->GetPosition();
	cubeCollider.rx = 0.2f;
	cubeCollider.ry = 2.0f;
	cubeCollider.rz = 0.2f;
	Beanstalk1->GetComponent<PhysicsComponent>()->SetScale(Vec3(0.3f, 0.5f, 0.3f));
	Beanstalk1->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Beanstalk_obj"));
	Beanstalk1->GetComponent<PhysicsComponent>()->isStatic = true;
	Beanstalk1->AddComponent<CollisionComponent>(nullptr, cubeCollider);
	Beanstalk1->AddComponent<ShaderComponent>(CubeShader);
	Beanstalk1->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Beanstalk_mat"));
	Beanstalk1->AddComponent<TriggerComponent>(nullptr, 1.0f);
	AddOpaqueActor(Beanstalk1);
	physicsSystem.AddActor(Beanstalk1);
	collisionSystem.AddActor(Beanstalk1);


	Ref<Actor>Beanstalk2 = std::make_shared<Actor>(nullptr);
	Beanstalk2->AddComponent<PhysicsComponent>(nullptr, Vec3(-2.5f, -0.2f, 0.9f),/// pos
		QMath::angleAxisRotation(30.0f, Vec3(0.0f, 1.0f, 0.0f)),
		Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	cubeCollider.center = Beanstalk2->GetComponent<PhysicsComponent>()->GetPosition();
	cubeCollider.rx = 0.2f;
	cubeCollider.ry = 0.7f;
	cubeCollider.rz = 0.2f;
	Beanstalk2->GetComponent<PhysicsComponent>()->SetScale(Vec3(0.3f, 0.2f, 0.3f));
	Beanstalk2->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Beanstalk_obj"));
	Beanstalk2->GetComponent<PhysicsComponent>()->isStatic = true;
	Beanstalk2->AddComponent<CollisionComponent>(nullptr, cubeCollider);
	Beanstalk2->AddComponent<ShaderComponent>(CubeShader);
	Beanstalk2->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Beanstalk_mat"));
	Beanstalk2->AddComponent<TriggerComponent>(nullptr, 1.0f);
	AddOpaqueActor(Beanstalk2);
	physicsSystem.AddActor(Beanstalk2);
	collisionSystem.AddActor(Beanstalk2);



	Ref<Actor>Leaf3 = std::make_shared<Actor>(nullptr);
	Leaf3->AddComponent<PhysicsComponent>(nullptr, Vec3(-2.5f, 0.2f, 0.9f),/// pos
		QMath::angleAxisRotation(35.0f, Vec3(1.0f, 1.0f, 0.0f)),
		Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	Leaf3->GetComponent<PhysicsComponent>()->SetScale(Vec3(0.3f, 0.5f, 0.3f));
	Leaf3->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Leaf_obj"));
	Leaf3->GetComponent<PhysicsComponent>()->isStatic = true;
	Leaf3->AddComponent<ShaderComponent>(CubeShader);
	Leaf3->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Leaf_mat"));
	Leaf3->AddComponent<TriggerComponent>(nullptr, 1.0f);
	AddOpaqueActor(Leaf3);


	Ref<Actor>Leaf4 = std::make_shared<Actor>(nullptr);
	Leaf4->AddComponent<PhysicsComponent>(nullptr, Vec3(-2.5f, 0.5f, 0.0f),/// pos
		QMath::angleAxisRotation(90.0f, Vec3(1.0f, 1.0f, 0.0f)),
		Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	Leaf4->GetComponent<PhysicsComponent>()->SetScale(Vec3(0.7f, 0.7f, 0.7f));
	Leaf4->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Leaf_obj"));
	Leaf4->GetComponent<PhysicsComponent>()->isStatic = true;
	Leaf4->AddComponent<ShaderComponent>(CubeShader);
	Leaf4->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Leaf_mat"));
	Leaf4->AddComponent<TriggerComponent>(nullptr, 1.0f);
	AddOpaqueActor(Leaf4);



	Ref<Actor>Leaf1 = std::make_shared<Actor>(nullptr);
	Leaf1->AddComponent<PhysicsComponent>(nullptr, Vec3(2.3f, 1.5f, -1.55f),/// pos
		QMath::angleAxisRotation(35.0f, Vec3(1.0f, 1.0f, 0.0f)),
		Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	Leaf1->GetComponent<PhysicsComponent>()->SetScale(Vec3(0.3f, 0.5f, 0.3f));
	Leaf1->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Leaf_obj"));
	Leaf1->GetComponent<PhysicsComponent>()->isStatic = true;
	Leaf1->AddComponent<ShaderComponent>(CubeShader);
	Leaf1->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Leaf_mat"));
	Leaf1->AddComponent<TriggerComponent>(nullptr, 1.0f);
	AddOpaqueActor(Leaf1);


	Ref<Actor>Leaf2 = std::make_shared<Actor>(nullptr);
	Leaf2->AddComponent<PhysicsComponent>(nullptr, Vec3(3.5f, 3.5f, -2.7f),/// pos
		QMath::angleAxisRotation(35.0f, Vec3(1.0f, -1.0f, 0.0f)),
		Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	Leaf2->GetComponent<PhysicsComponent>()->SetScale(Vec3(0.7f, 0.7f, 0.7f));
	Leaf2->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Leaf_obj"));
	Leaf2->GetComponent<PhysicsComponent>()->isStatic = true;
	Leaf2->AddComponent<ShaderComponent>(CubeShader);
	Leaf2->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Leaf_mat"));
	Leaf2->AddComponent<TriggerComponent>(nullptr, 1.0f);
	AddOpaqueActor(Leaf2);


	Ref<Actor>Flower_5 = std::make_shared<Actor>(nullptr);
	Flower_5->AddComponent<PhysicsComponent>(nullptr, Vec3(1.3f, -1.5f, 0.3f),/// pos
		QMath::angleAxisRotation(0.0f, Vec3(-1.0f, 1.0f, 0.0f)),
		Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	Flower_5->GetComponent<PhysicsComponent>()->SetScale(Vec3(0.15f, 0.15f, 0.15f));
	Flower_5->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Flower_obj"));
	Flower_5->GetComponent<PhysicsComponent>()->isStatic = true;
	Flower_5->AddComponent<ShaderComponent>(CubeShader);
	Flower_5->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Flower_v1_mat"));
	Flower_5->AddComponent<TriggerComponent>(nullptr, 1.0f);
	AddOpaqueActor(Flower_5);


	Ref<Actor>Tower1 = std::make_shared<Actor>(nullptr);
	Tower1->AddComponent<PhysicsComponent>(nullptr, Vec3(-5.3f, -5.0f, 0.3f),/// pos
		QMath::angleAxisRotation(0.0f, Vec3(-1.0f, 1.0f, 0.0f)),
		Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	Tower1->GetComponent<PhysicsComponent>()->SetScale(Vec3(0.5f, 0.5f, 0.5f));
	Tower1->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Tower_obj"));
	Tower1->GetComponent<PhysicsComponent>()->isStatic = true;
	Tower1->AddComponent<ShaderComponent>(CubeShader);
	Tower1->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Tower_mat"));
	Tower1->AddComponent<TriggerComponent>(nullptr, 1.0f);
	AddOpaqueActor(Tower1);


	Ref<Actor>Bridge = std::make_shared<Actor>(nullptr);
	Bridge->AddComponent<PhysicsComponent>(nullptr, Vec3(-6.3f, -5.5f, 0.3f),/// pos
		QMath::angleAxisRotation(0.0f, Vec3(-1.0f, 1.0f, 0.0f)),
		Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	Bridge->GetComponent<PhysicsComponent>()->SetScale(Vec3(0.3f, 0.3f, 0.3f));
	Bridge->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Bridge_obj"));
	Bridge->GetComponent<PhysicsComponent>()->isStatic = true;
	Bridge->AddComponent<ShaderComponent>(CubeShader);
	Bridge->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Bridge_mat"));
	Bridge->AddComponent<TriggerComponent>(nullptr, 1.0f);
	AddOpaqueActor(Bridge);




return true;
}

void CapstoneSceneDream::Update(const float deltaTime) {
	DrawUI_imgui();
	dialogueSystem->Render();



	PlayerGroundCheck();
	iTime += deltaTime;
	interactionManager->Reset();

	//change in angle 
	if (rotatePlayerLeft) {
		playerAngle -= 0.4f;
	}
	else if (rotatePlayerRight) {
		playerAngle += 0.4f;
	}

	Ref<PhysicsComponent> playerPhysics = player->GetComponent<PhysicsComponent>();
	//Reset the player when below a specific Y value
	if (playerPhysics->GetPosition().y < -1000) {
		playerPhysics->SetVel(Vec3());
		playerPhysics->SetPosition(Vec3());
		playerPhysics->useGravity = false;
		anyInputPressed = false;

	}

	//Update players rotation 
	if (rotatePlayerLeft || rotatePlayerRight) {
		Quaternion newQuat = QMath::angleAxisRotation(playerAngle, Vec3(0.0f, 1.0f, 0.0f));
		playerPhysics->SetQuaternion(newQuat);
	}

	if (VMath::mag(movementInput) > 0.0f || !playerIsGrounded || playerIsSwimming) {

	}

	currentTime += deltaTime;
	if (!playerIsGrounded) {
		currentAnim = PlayerAnimType::Jumping;
	}
	else if (VMath::mag(movementInput) > 0.0f) {
		currentAnim = PlayerAnimType::Walking;
	}
	else {
		currentAnim = PlayerAnimType::Idle;
	}


	/// Adriel's movement input
	Vec3 moveDir = Vec3();
	if (VMath::mag(movementInput) > 0.0f) {
		//Capping the x & z directional movement
		moveDir = VMath::normalize(movementInput);
		//Y is the exceptions because you are jumping there
		moveDir.y = movementInput.y;
	}

	animIndex = GetAnimIndex(deltaTime, currentTime, currentAnim, frameSpeed);
	Vec3 moveResult;
	if (underwater) {
		if(anyInputPressed) playerPhysics->useGravity = false;
		playerPhysics->SetVel(playerPhysics->GetVel() * 0.98f);
		moveResult = moveDir * swimSpeed;
		playerPhysics->ApplyForce(moveResult);
	}
	else {
		if(anyInputPressed) playerPhysics->useGravity = true;
		//Change accel depending if the player is grounded or not
		if (playerIsGrounded) {
			walkSpeed = groudAccel;
		}
		else {
			walkSpeed = airAccel;
		}
		moveResult = moveDir * walkSpeed;
		moveResult.y = movementInput.y * jumpSpeed;
		playerPhysics->ApplyForce(moveResult);
		movementInput.y = 0; //Reseting the jumping input

		if (playerPhysics->GetVel().y > jumpSpeed * deltaTime) {
			Vec3 currentVel = playerPhysics->GetVel();
			playerPhysics->SetVel(Vec3(currentVel.x, jumpSpeed * deltaTime, currentVel.z));
		}
	}
	NPCcurrentTime += deltaTime * 0.4f;
	NPCanimIndex = static_cast<int>(NPCcurrentTime / frameSpeed) % 17;

	camera->UpdateViewMatrix();
	collisionSystem.Update(deltaTime);
	physicsSystem.Update(deltaTime);

	underwater = false;
	triggerSystem.Update(deltaTime);
}


void CapstoneSceneDream::InitializeDialogue()
{
	dialogueSequences.resize(2);
	unsigned int textureID = assetManager->GetComponent<MaterialComponent>("Player_smile")->getTextureID();
	Dialogue mermaid_d1("Mermaid", "Have you found something from the sea?", textureID);
	dialogueSequences[1].push_back(mermaid_d1);
}

void CapstoneSceneDream::Render() const {
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

		glEnable(GL_DEPTH_TEST);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		DebugUI();

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

		glDisable(GL_BLEND);
		//glDepthFunc(GL_LESS);

		for (auto opaqueActor : opaqueActors) {
			glUseProgram(opaqueActor->GetComponent<ShaderComponent>()->GetProgram());
			//glUniform1f(opaqueActor->GetComponent<ShaderComponent>()->GetUniformID("index"), animIndex);
			glUniformMatrix4fv(opaqueActor->GetComponent<ShaderComponent>()->GetUniformID("modelMatrix"), 1, GL_FALSE, opaqueActor->GetModelMatrix());
			glBindTexture(GL_TEXTURE_2D, opaqueActor->GetComponent<MaterialComponent>()->getTextureID());
			opaqueActor->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
		}

		RenderColliders();
		std::vector<Ref<Actor>> sortedTransparentActors = transparentActors;
		std::sort(sortedTransparentActors.begin(), sortedTransparentActors.end(),
			[this](const Ref<Actor>& a, const Ref<Actor>& b) {
				// Extract camera position from the orientation matrix
				Vec3 camPos = camera->GetComponent<TransformComponent>()->GetPosition();

				// Get positions of actors
				Vec3 posA = a->GetComponent<PhysicsComponent>()->GetPosition();
				Vec3 posB = b->GetComponent<PhysicsComponent>()->GetPosition();

				// Calculate squared distances (faster than computing actual distances)
				float distSquaredA = VMath::dot(posA - camPos, posA- camPos);
				float distSquaredB = VMath::dot(posB - camPos, posB - camPos);

				if (a->NPCid == 0) return false;
				if (b->NPCid == 0) return true;
				// Sort from far to near
				return distSquaredA > distSquaredB;
			});


		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_TRUE);
		glEnable(GL_CULL_FACE);
		//glCullFace(GL_FRONT);

		Matrix4 cameraWorldMatrix = camera->orient; // This is the camera's world transform
		Vec3 cameraPosition = Vec3(cameraWorldMatrix[12], cameraWorldMatrix[13], cameraWorldMatrix[14]);

		glUseProgram(Ocean->GetComponent<ShaderComponent>()->GetProgram());

		glUniformMatrix4fv(Ocean->GetComponent<ShaderComponent>()->GetUniformID("modelMatrix"), 1, GL_FALSE, Ocean->GetModelMatrix());
		glUniformMatrix4fv(Ocean->GetComponent<ShaderComponent>()->GetUniformID("viewMatrix"), 1, GL_FALSE, MMath::inverse(camera->orient));
		glUniformMatrix4fv(Ocean->GetComponent<ShaderComponent>()->GetUniformID("projectionMatrix"), 1, GL_FALSE, camera->GetProjectionMatrix());
		glUniform3fv(Ocean->GetComponent<ShaderComponent>()->GetUniformID("cameraPos"), 1, cameraPosition);
		glUniform1f(Ocean->GetComponent<ShaderComponent>()->GetUniformID("time"), currentTime);

		// Bind the texture and assign to sampler2D
		if (Ocean->GetComponent<MaterialComponent>()) {
			glActiveTexture(GL_TEXTURE0); // Select texture unit 0
			glBindTexture(GL_TEXTURE_2D, Ocean->GetComponent<MaterialComponent>()->getTextureID()); // Bind water normal map
			glUniform1i(Ocean->GetComponent<ShaderComponent>()->GetUniformID("waterNormalMap"), 0); // Set uniform sampler to use texture unit 0

			Ocean->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
		}




		glDepthMask(GL_TRUE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_BLEND);

		for (auto transparentActor : sortedTransparentActors) {
			glUseProgram(transparentActor->GetComponent<ShaderComponent>()->GetProgram());
			glUniform1i(transparentActor->GetComponent<ShaderComponent>()->GetUniformID("NPC_id"), transparentActor->NPCid);
			glUniform1i(transparentActor->GetComponent<ShaderComponent>()->GetUniformID("talking"), true);
			glUniform2f(transparentActor->GetComponent<ShaderComponent>()->GetUniformID("playeranimIndex"), animIndex.x, animIndex.y);
			glUniform2f(transparentActor->GetComponent<ShaderComponent>()->GetUniformID("NPCanimIndex"), NPCanimIndex, 0);

			glUniformMatrix4fv(transparentActor->GetComponent<ShaderComponent>()->GetUniformID("modelMatrix"), 1, GL_FALSE, transparentActor->GetModelMatrix());
			if (transparentActor->GetComponent<MaterialComponent>()) {
				glBindTexture(GL_TEXTURE_2D, transparentActor->GetComponent<MaterialComponent>()->getTextureID());
				transparentActor->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
			}
		}


		
		


	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glUseProgram(assetManager->GetComponent<ShaderComponent>("Frame buffer")->GetProgram());
	//glUniform1f(assetManager->GetComponent<ShaderComponent>("Frame buffer")->GetUniformID("time"), currentTime);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	////rendering the screen (triangle)
	//glBindVertexArray(VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	//glBindVertexArray(0);


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void CapstoneSceneDream::DrawUI_imgui()
{
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

		/// Drawing the object texture on top
		if (sceneMan->inventory.items[0] != nullptr) {
			ImGui::SetNextWindowPos(ImVec2(285, 625)); // Window position + 35
			ImGui::SetNextWindowSize(ImVec2(150, 150)); // Set the window size
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::Begin("Item slot 1", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration);
			ImGui::Image((intptr_t)sceneMan->inventory.items[0]->GetComponent<MaterialComponent>()->getTextureID(), ImVec2(80, 80));
			ImGui::End();
			ImGui::PopStyleVar();
			ImGui::PopStyleColor();
		}
		ImGui::SetNextWindowPos(ImVec2(285 + 35, 625 + 35)); // Set the window position here (x, y)
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

		if (sceneMan->inventory.items[1] != nullptr) {
			ImGui::SetNextWindowPos(ImVec2(395 + 35, 665 + 35));
			ImGui::SetNextWindowSize(ImVec2(150, 150));
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::Begin("Item slot 2", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration);
			ImGui::Image((intptr_t)sceneMan->inventory.items[1]->GetComponent<MaterialComponent>()->getTextureID(), ImVec2(80, 80));
			ImGui::End();
			ImGui::PopStyleVar();
			ImGui::PopStyleColor();
		}

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

		if (sceneMan->inventory.items[2] != nullptr) {
			ImGui::SetNextWindowPos(ImVec2(465 + 35, 815 + 35)); // Set the window position here (x, y)
			ImGui::SetNextWindowSize(ImVec2(150, 150)); // Set the window size
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::Begin("Item slot 3", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration);
			ImGui::Image((intptr_t)sceneMan->inventory.items[2]->GetComponent<MaterialComponent>()->getTextureID(), ImVec2(80, 80));
			ImGui::End();
			ImGui::PopStyleVar();
			ImGui::PopStyleColor();
		}

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

void CapstoneSceneDream::CreateDebugMeshes() {
	//Creating the debug objects
	DebugSphere = std::make_shared<Actor>(nullptr);
	DebugSphere->AddComponent<TransformComponent>(nullptr, Vec3(), Quaternion(), Vec3());
	DebugSphere->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Sphere"));
	DebugSphere->AddComponent<ShaderComponent>(assetManager->GetComponent< ShaderComponent>("DefaultShader"));

	DebugCube = std::make_shared<Actor>(nullptr);
	DebugCube->AddComponent<TransformComponent>(nullptr, Vec3(), Quaternion(), Vec3());
	DebugCube->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Cube"));
	DebugCube->AddComponent<ShaderComponent>(assetManager->GetComponent< ShaderComponent>("DefaultShader"));
}

void CapstoneSceneDream::DrawSphere(Vec3 pos, float radius) const {
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
void CapstoneSceneDream::DrawSphere(Sphere s) const {
	DrawSphere(s.center, s.r);
}

void CapstoneSceneDream::DrawCube(Vec3 pos, Vec3 dimensions) const {
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
void CapstoneSceneDream::DrawCube(AABB a) const {
	DrawCube(a.center, Vec3(a.rx, a.ry, a.rz));
}

void CapstoneSceneDream::DrawRay(Ray ray) const {
	Vec3 center = ray.start + (ray.direction * 0.5f);
	float length = VMath::mag(ray.direction);
	Vec3 dimensions = Vec3(length, 0.05, 0.05);

	Vec3 angleAxis = VMath::cross(Vec3(1, 0, 0), VMath::normalize(ray.direction));
	float angle = acos(VMath::dot(Vec3(1, 0, 0), VMath::normalize(ray.direction))) * RADIANS_TO_DEGREES;

	Quaternion rotation = QMath::angleAxisRotation(angle, angleAxis);
	Matrix4 modelMatrix = MMath::translate(center) * MMath::toMatrix4(rotation) * MMath::scale(dimensions);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glUseProgram(DebugCube->GetComponent<ShaderComponent>()->GetProgram());
	glUniformMatrix4fv(DebugCube->GetComponent<ShaderComponent>()->GetUniformID("projectionMatrix"), 1, GL_FALSE, camera->GetProjectionMatrix());
	glUniformMatrix4fv(DebugCube->GetComponent<ShaderComponent>()->GetUniformID("viewMatrix"), 1, GL_FALSE, camera->GetViewMatrix());
	glUniformMatrix4fv(DebugCube->GetComponent<ShaderComponent>()->GetUniformID("modelMatrix"), 1, GL_FALSE, modelMatrix);

	DebugCube->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void CapstoneSceneDream::PlayerGroundCheck() {
	playerIsGrounded = false;
	float length = 0.5f;

	Vec3 playerPos = player->GetComponent<TransformComponent>()->GetPosition();
	Vec3 origin = playerPos + Vec3(0, -0.3, 0);
	Ray ray = Ray(origin, origin + Vec3(0, -1, 0) * length);
	groundCheckRay = ray;

	std::vector<Ref<Actor>> collidedActors = collisionSystem.Raycast(groundCheckRay);
	
	//Sotring all the collided actors in a string to print
	rayCollidedActors = "";
	for (auto actor : collidedActors) {
		if (actor->tag == TAGS::GROUND) {
			std::stringstream ss;
			Vec3 pos = actor->GetComponent<TransformComponent>()->GetPosition();

			rayCollidedActors += "Collided with actor at: ";
			ss << "(" << pos.x << ", " << pos.y << ", " << pos.z << ")" << "\n";
			rayCollidedActors += ss.str();
		}
	}

	for (auto actor : collidedActors) {
		if (actor->tag == TAGS::GROUND) {
			playerIsGrounded = true;
			//std::cout << "Player is grounded \n";
			return;
		}
	}
	//std::cout << "Player is NOT grounded \n";
}

void CapstoneSceneDream::RenderColliders() const {
	//Drawing the triggers
	for (auto trigger : triggerSystem.triggeringActors) {
		Ref<TriggerComponent> tc = trigger->GetComponent<TriggerComponent>();
		switch (tc->colliderType) {
			case TriggerType::Sphere:
				DrawSphere(trigger->GetComponent<TransformComponent>()->GetPosition(), trigger->GetComponent<TriggerComponent>()->radius);
				break;

			case TriggerType::Box:
				DrawCube(trigger->GetComponent<TransformComponent>()->GetPosition(), Vec3(tc->box.rx, tc->box.ry, tc->box.rz));
				break;
		}
	}

	//Drawing the colliders
	for (auto actor : collisionSystem.collidingActors) {
		Ref<CollisionComponent> collider = actor->GetComponent<CollisionComponent>();
		switch (collider->GetColliderType()) {
			case ColliderType::Sphere:
				DrawSphere(actor->GetComponent<TransformComponent>()->GetPosition(), collider->GetRadisu());
				break;
			case ColliderType::AABB:
				DrawCube(collider->GetAABB());
				break;
		default:
			break;
		}
	}

	//Drawing the player's ground ray
	//DrawRay(groundCheckRay);
}

void CapstoneSceneDream::DebugUI() const {
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiCond_FirstUseEver | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;
	ImGui::SetNextWindowPos(ImVec2());
	ImGui::SetNextWindowSize(ImVec2(400, 400));
	ImGui::Begin("DEBUG", nullptr, flags);
	
	//Displaying the player's position
	Vec3 playerPos = player->GetComponent<TransformComponent>()->GetPosition();
	ImGui::Text("Player Pos: (%.3f, %.3f, %.3f)", playerPos.x, playerPos.y, playerPos.z);
	ImGui::Text("Player Grounded: %s", (playerIsGrounded) ? "True" : "False");
	ImGui::Text("Player Underwater: %s", (underwater) ? "True" : "False");

	Vec3 playerForce = player->GetComponent<PhysicsComponent>()->GetForce();
	ImGui::Text("Player Force: (%.3f, %.3f, %.3f)", playerForce.x, playerForce.y, playerForce.z);

	Vec3 playerAccel = player->GetComponent<PhysicsComponent>()->GetAcc();
	ImGui::Text("Player Accel: (%.3f, %.3f, %.3f)", playerAccel.x, playerAccel.y, playerAccel.z);

	Vec3 playerVel = player->GetComponent<PhysicsComponent>()->GetVel();
	ImGui::Text("Player Vel: (%.3f, %.3f, %.3f)", playerVel.x, playerVel.y, playerVel.z);

	//Printing the ray's positions
	Vec3 rayOrigin = groundCheckRay.start;
	Vec3 rayEnd = groundCheckRay.direction;

	ImGui::Text("Ray Origin: (%.3f, %.3f, %.3f)", rayOrigin.x, rayOrigin.y, rayOrigin.z);
	ImGui::Text("Ray End   : (%.3f, %.3f, %.3f)", rayEnd.x, rayEnd.y, rayEnd.z);

	ImGui::SeparatorText("CollidedActors");
	ImGui::Text("%s", rayCollidedActors.c_str());

	ImGui::End();

	Vec2 rayOriginScreen = sceneManagerRef->WorldToScreenCoordinates(rayOrigin, camera.get());
	Vec2 rayEndScreen = sceneManagerRef->WorldToScreenCoordinates(rayEnd, camera.get());

	// S = start
	ImGui::SetNextWindowPos(ImVec2(rayOriginScreen.x, rayOriginScreen.y));
	ImGui::SetNextWindowSize(ImVec2(0.3f, 0.3f));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::Begin("s", nullptr, flags);
	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();

	// E = end
	ImGui::SetNextWindowPos(ImVec2(rayEndScreen.x, rayEndScreen.y));
	ImGui::SetNextWindowSize(ImVec2(0.3f, 0.3f));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::Begin("e", nullptr, flags);
	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
}

void CapstoneSceneDream::PendItemToInventory(std::shared_ptr<Actor> other)
{
	Ref<PickableItem> item;
	item = std::dynamic_pointer_cast<PickableItem>(other);
	if (item != nullptr) {

		//Originally I wanted to add a check to see if the item is in the inventory before it's allowed to be Pend
		//But removing it from the triggerSystem array should do the trick (hopefully)
		//for(int i = 0; i < inventory->inventorySize; i++){}

		inventory->pendingItem = nullptr;
		inventory->pendingItem = item;
	}
}


void CapstoneSceneDream::AddItemToInventory(std::shared_ptr<Actor> other, int index) {
	//Check if it's a pickable item
	if (std::dynamic_pointer_cast<PickableItem>(other) != nullptr) {
		//Drops the item
		if (inventory->items[index] != nullptr) {
			DropItemFromInventory(index);
		}

		//Add the item to the inventory
		inventory->AddItem(std::dynamic_pointer_cast<PickableItem>(other), index);

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
		index = 0;
		for (int i = 0; i < opaqueActors.size(); i++) {
			if (opaqueActors[i] == other) {
				index = i;
				break;
			}
		}
		opaqueActors.erase(opaqueActors.begin() + index);
	}
}


void CapstoneSceneDream::DropItemFromInventory(int index) {
	//Add the object back into the actor vectors
	transparentActors.push_back(inventory->items[index]);
	triggerSystem.AddActor(inventory->items[index]);

	//Remove the item from the inventory
	inventory->RemoveItem(index);
}


void CapstoneSceneDream::PlayerTriggerCallback(Ref<Actor> other) {
	//Check if it's an item
	Ref<PickableItem> item = std::dynamic_pointer_cast<PickableItem>(other);
	if (item != nullptr) {
		PendItemToInventory(other);
		return;
	}

	//Check if it's an interactable actor
	Ref<InteractableActor> actor = std::dynamic_pointer_cast<InteractableActor>(other);
	if (actor != nullptr) {
		interactionManager->SetCurrentInteraction(actor);
		return;
	}
}


