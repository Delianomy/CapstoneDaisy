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
	Ref<ShaderComponent> CubeShader = assetManager->GetComponent<ShaderComponent>("RegularTextureShader");


	AdrielMagik();

	mermaid = std::make_shared<Actor>(nullptr);
	mermaid->NPCid = 1;
	mermaid->AddComponent<PhysicsComponent>(nullptr, Vec3(1.0f, 0.0f, 0.0f),/// pos
		QMath::angleAxisRotation(0.0f, Vec3(1.0f, 0.0f, 0.0f)),
		Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	mermaid->GetComponent<PhysicsComponent>()->SetScale(Vec3(2.0f, 2.0f, 1.0f));
	/// This makes a Sphere Collision Component because of the argument list - just the radius. 
	mermaid->AddComponent<CollisionComponent>(nullptr, 0.8f);
	mermaid->GetComponent<PhysicsComponent>()->isStatic = true;
	mermaid->GetComponent<PhysicsComponent>()->mass = 1.0f;
	mermaid->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Square"));
	mermaid->AddComponent<ShaderComponent>(shader);
	mermaid->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Mermaid"));
	mermaid->AddComponent<TriggerComponent>(nullptr, 1.0f);
	AddTransparentActor(mermaid);


	mrOwl = std::make_shared<Actor>(nullptr);
	mrOwl->NPCid = 2;
	mrOwl->AddComponent<PhysicsComponent>(nullptr, Vec3(3.0f, 0.0f, 0.0f),/// pos
		QMath::angleAxisRotation(0.0f, Vec3(1.0f, 0.0f, 0.0f)),
		Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	mrOwl->GetComponent<PhysicsComponent>()->SetScale(Vec3(1.3f, 1.3f, 1.0f));
	/// This makes a Sphere Collision Component because of the argument list - just the radius. 
	mrOwl->AddComponent<CollisionComponent>(nullptr, 0.8f);
	mrOwl->GetComponent<PhysicsComponent>()->isStatic = true;
	mrOwl->GetComponent<PhysicsComponent>()->mass = 1.0f;
	mrOwl->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Square"));
	mrOwl->AddComponent<ShaderComponent>(shader);
	mrOwl->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Owl"));
	mrOwl->AddComponent<TriggerComponent>(nullptr, 1.0f);
	AddTransparentActor(mrOwl);


	mrsMouse = std::make_shared<Actor>(nullptr);
	mrsMouse->NPCid = 3;
	mrsMouse->AddComponent<PhysicsComponent>(nullptr, Vec3(8.5f, -0.13f, 0.3f),/// pos
		QMath::angleAxisRotation(0.0f, Vec3(1.0f, 0.0f, 0.0f)),
		Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	mrsMouse->GetComponent<PhysicsComponent>()->SetScale(Vec3(0.9f, 0.9f, 1.0f));
	/// This makes a Sphere Collision Component because of the argument list - just the radius. 
	mrsMouse->AddComponent<CollisionComponent>(nullptr, 0.8f);
	mrsMouse->GetComponent<PhysicsComponent>()->isStatic = true;
	mrsMouse->GetComponent<PhysicsComponent>()->mass = 1.0f;
	mrsMouse->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Square"));
	mrsMouse->AddComponent<ShaderComponent>(shader);
	mrsMouse->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Mouse"));
	mrsMouse->AddComponent<TriggerComponent>(nullptr, 1.0f);
	AddTransparentActor(mrsMouse);


	fairy = std::make_shared<Actor>(nullptr);
	fairy->NPCid = 4;
	fairy->AddComponent<PhysicsComponent>(nullptr, Vec3(5.0f, 0.0f, 0.0f),/// pos
		QMath::angleAxisRotation(0.0f, Vec3(1.0f, 0.0f, 0.0f)),
		Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	fairy->GetComponent<PhysicsComponent>()->SetScale(Vec3(1.0f, 1.0f, 1.0f));
	fairy->AddComponent<CollisionComponent>(nullptr, 0.8f);
	fairy->GetComponent<PhysicsComponent>()->isStatic = true;
	fairy->GetComponent<PhysicsComponent>()->mass = 1.0f;
	fairy->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Square"));
	fairy->AddComponent<ShaderComponent>(shader);
	fairy->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Fairy"));
	fairy->AddComponent<TriggerComponent>(nullptr, 1.0f);
	AddTransparentActor(fairy);

	//make an actor
	player = std::make_shared<Actor>(nullptr);
	player->NPCid = 0;
	player->AddComponent<PhysicsComponent>(nullptr, Vec3(0.0f, 0.0f, 0.0f),/// pos
		QMath::angleAxisRotation(0.0f, Vec3(1.0f, 0.0f, 0.0f)),
		Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	player->GetComponent<PhysicsComponent>()->SetScale(Vec3(1.0f, 1.0f, 1.0f));
	/// This makes a Sphere Collision Component because of the argument list - just the radius. 
	player->AddComponent<CollisionComponent>(nullptr, 0.8f);
	player->GetComponent<PhysicsComponent>()->isStatic = false;
	player->GetComponent<PhysicsComponent>()->useGravity = false;
	player->GetComponent<PhysicsComponent>()->mass = 1.0f;
	player->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Square"));
	player->AddComponent<ShaderComponent>(shader);
	player->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Daisy_spriteSheet"));
	player->AddComponent<TriggerComponent>(nullptr, 1.0f);
	player->GetComponent<TriggerComponent>()->SetCallback(TriggerCallbackCreator::CreateTriggerCallback(this, &CapstoneSceneDream::PrintStatement));
	AddTransparentActor(player);


	//now technically it would mean that out player now has a collider
	GLint maxTextureSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
	printf("Max Texture Size: %d\n", maxTextureSize);

	GLint totalMemoryKB;
	glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &totalMemoryKB);
	printf("Available GPU Memory: %d KB\n", totalMemoryKB);

	camera = std::make_shared<CameraActor>(player.get());
	camera->isInMainMenu = false;
	camera->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, -2.0f), Quaternion());
	camera->OnCreate();
	camera->GetProjectionMatrix().print("ProjectionMatrix");
	camera->GetViewMatrix().print("ViewMatrix");

	//cube = std::make_shared<Actor>(nullptr);
	//cube->AddComponent<PhysicsComponent>(nullptr, Vec3(0.0f, -2.0f, 0.0f),/// pos
	//	QMath::angleAxisRotation(0.0f, Vec3(1.0f, 0.0f, 0.0f)),
	//	Vec3(0.0f, 0.0f, 0.0f) ///velocity
	//);
	//cube->GetComponent<PhysicsComponent>()->SetScale(Vec3(1.0f, 1.0f, 1.0f));
	//cube->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("House"));
	//AABB cubeCollider;
	//cubeCollider.center = cube->GetComponent<PhysicsComponent>()->GetPosition();
	////Problem, this looks a bit weird cause Y goes very deep in the bottom comparing to the top side, leadingfor stuff to look sketchy 
	//cubeCollider.rx = 3.18f;
	//cubeCollider.ry = 0.51f;
	//cubeCollider.rz = 0.904f;

	//cube->AddComponent<CollisionComponent>(nullptr, cubeCollider);
	//cube->GetComponent<PhysicsComponent>()->isStatic = true;

	//cube->AddComponent<ShaderComponent>(CubeShader);
	//cube->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("house"));
	//cube->AddComponent<TriggerComponent>(nullptr, 1.0f);
	//AddOpaqueActor(cube);

	skybox = std::make_shared<SkyBox>(nullptr, "textures/Skyboxes/Overworld/px.png", "textures/Skyboxes/Overworld/nx.png",
		"textures/Skyboxes/Overworld/py.png", "textures/Skyboxes/Overworld/ny.png", "textures/Skyboxes/Overworld/pz.png",
		"textures/Skyboxes/Overworld/nz.png");

	skybox->OnCreate();

	light = std::make_shared<LightActor>(camera.get(), LightStyle::DirectionLight, Vec3(0.0f, 5.0f, 1.0f), Vec4(0.85f, 0.6, 0.6f, 0.0f));
	light->OnCreate();

	CreateLevelLayout();



	/// Register the two balls with the physics and collision systems
	physicsSystem.AddActor(player);

	

	collisionSystem.AddActor(player);



	triggerSystem.AddActor(player);


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

bool CapstoneSceneDream::CreateLevelLayout() {
	Ref<ShaderComponent> CubeShader = assetManager->GetComponent<ShaderComponent>("RegularTextureShader");

	Island1 = std::make_shared<Actor>(nullptr);
	Island1->AddComponent<PhysicsComponent>(nullptr, Vec3(0.0f, -2.0f, 0.0f),/// pos
		QMath::angleAxisRotation(90.0f, Vec3(0.0f, 1.0f, 0.0f)),
		Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	Island1->GetComponent<PhysicsComponent>()->SetScale(Vec3(1.0f, 1.0f, 1.0f));
	Island1->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Island1_obj"));
	AABB cubeCollider;
	cubeCollider.center = Island1->GetComponent<PhysicsComponent>()->GetPosition();
	//Problem, this looks a bit weird cause Y goes very deep in the bottom comparing to the top side, leadingfor stuff to look sketchy 
	cubeCollider.rx = 3.18f;
	cubeCollider.ry = 0.51f;
	cubeCollider.rz = 0.904f;


	Island1->AddComponent<CollisionComponent>(nullptr, cubeCollider);
	Island1->GetComponent<PhysicsComponent>()->isStatic = true;

	Island1->AddComponent<ShaderComponent>(CubeShader);
	Island1->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Island1_mat"));
	Island1->AddComponent<TriggerComponent>(nullptr, 1.0f);
	AddOpaqueActor(Island1);




	Island2 = std::make_shared<Actor>(nullptr);
	Island2->AddComponent<PhysicsComponent>(nullptr, Vec3(18.0f, -2.0f, 0.0f),/// pos
		QMath::angleAxisRotation(0.0f, Vec3(1.0f, 0.0f, 0.0f)),
		Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	Island2->GetComponent<PhysicsComponent>()->SetScale(Vec3(1.0f, 0.7f, 1.0f));
	Island2->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Island2_obj"));
	cubeCollider;
	cubeCollider.center = Island2->GetComponent<PhysicsComponent>()->GetPosition();
	//Problem, this looks a bit weird cause Y goes very deep in the bottom comparing to the top side, leadingfor stuff to look sketchy 
	cubeCollider.rx = 3.18f;
	cubeCollider.ry = 0.51f;
	cubeCollider.rz = 0.904f;


	//cube->AddComponent<CollisionComponent>(nullptr, cubeCollider);
	Island2->GetComponent<PhysicsComponent>()->isStatic = true;

	Island2->AddComponent<ShaderComponent>(CubeShader);
	Island2->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Island2_mat"));
	Island2->AddComponent<CollisionComponent>(nullptr, 1.0f);
	AddOpaqueActor(Island2);
	physicsSystem.AddActor(Island2);
	collisionSystem.AddActor(Island2);


	House = std::make_shared<Actor>(nullptr);
	House->AddComponent<PhysicsComponent>(nullptr, Vec3(8.8f, 0.4f, -0.5f),/// pos
		QMath::angleAxisRotation(0.0f, Vec3(1.0f, 0.0f, 0.0f)),
		Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	House->GetComponent<PhysicsComponent>()->SetScale(Vec3(1.0f, 1.0f, 1.0f));
	House->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("House_obj"));
	 cubeCollider;
	cubeCollider.center = House->GetComponent<PhysicsComponent>()->GetPosition();
	//Problem, this looks a bit weird cause Y goes very deep in the bottom comparing to the top side, leadingfor stuff to look sketchy 
	cubeCollider.rx = 1.0f;
	cubeCollider.ry = 1.0f;
	cubeCollider.rz = 1.0f;


	House->AddComponent<CollisionComponent>(nullptr, 1.0);
	House->GetComponent<PhysicsComponent>()->isStatic = true;

	House->AddComponent<ShaderComponent>(CubeShader);
	House->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("House_mat"));
	House->AddComponent<TriggerComponent>(nullptr, 1.0f);
	AddOpaqueActor(House);
	physicsSystem.AddActor(House);
	collisionSystem.AddActor(House);




	Flower_1 = std::make_shared<Actor>(nullptr);
	Flower_1->AddComponent<PhysicsComponent>(nullptr, Vec3(8.8f, -1.25f, 2.0f),/// pos
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



	Flower_2 = std::make_shared<Actor>(nullptr);
	Flower_2->AddComponent<PhysicsComponent>(nullptr, Vec3(11.3f, -1.25f, 2.8f),/// pos
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


	Flower_3 = std::make_shared<Actor>(nullptr);
	Flower_3->AddComponent<PhysicsComponent>(nullptr, Vec3(12.3f, -0.92f, 1.3f),/// pos
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


	Flower_4 = std::make_shared<Actor>(nullptr);
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


	Beanstalk1 = std::make_shared<Actor>(nullptr);
	Beanstalk1->AddComponent<PhysicsComponent>(nullptr, Vec3(1.3f, 0.8f, -0.9f),/// pos
		QMath::angleAxisRotation(0.0f, Vec3(-1.0f, 1.0f, 0.0f)),
		Vec3(0.0f, 0.0f, 0.0f) ///velocity
	);
	Beanstalk1->GetComponent<PhysicsComponent>()->SetScale(Vec3(0.3f, 0.5f, 0.3f));
	Beanstalk1->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("Beanstalk_obj"));
	Beanstalk1->GetComponent<PhysicsComponent>()->isStatic = true;
	Beanstalk1->AddComponent<ShaderComponent>(CubeShader);
	Beanstalk1->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("Beanstalk_mat"));
	Beanstalk1->AddComponent<TriggerComponent>(nullptr, 1.0f);
	AddOpaqueActor(Beanstalk1);



	Leaf1 = std::make_shared<Actor>(nullptr);
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


	Leaf2 = std::make_shared<Actor>(nullptr);
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


	Flower_5 = std::make_shared<Actor>(nullptr);
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

	return true;
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
	



	//Vec3 movement(0.0f, 0.0f, 0.0f);
	//if (goRight) {
	//	
	//	movement.x += 1.0f;
	//}
	//if (goLeft) {
	//	movement.x -= 1.0f;
	//}
	//if (goForward) {
	//	movement.z += 1.0f;
	//}
	//if (goBackwards) {
	//	movement.z -= 1.0f;
	//}

	//normalize the movement to remove the diagonal speed up
	if (VMath::mag(movementInput) > 0.0f) {
		//VMath::normalize(movement);
		currentTime += deltaTime;
		animIndex = static_cast<int>(currentTime / frameSpeed) % 13;
		//Quaternion currentRot = playerPhysics->GetQuaternion();
		//Matrix4 currentRotMatrix = MMath::toMatrix4(currentRot);
		//Vec3 moveDirection = Vec3();
		//if (movement.x != 0) {
		//	moveDirection += currentRotMatrix * lefr_right_Vector * movement.x * walkSpeed;
		//}
		//if (movement.z != 0.0f) {
		//	moveDirection += currentRotMatrix * forwardVector * movement.z * walkSpeed;
		//}
		//playerPhysics->ApplyForce(moveDirection);
	}
	else {
		//playerPhysics->SetVel(Vec3());
		animIndex = 0;
	}
	
	Vec3 moveDir = Vec3();
	if (VMath::mag(movementInput) > 0.0f) {
		//Capping the x & z directional movement
		moveDir = VMath::normalize(movementInput);

		//Y is the exceptions because you are jumping there
		moveDir.y = movementInput.y;
	}

	playerPhysics->ApplyForce(moveDir * walkSpeed * 0.1);

	NPCcurrentTime += deltaTime*0.4f;
	NPCanimIndex = static_cast<int>(NPCcurrentTime / frameSpeed) % 17;

	camera->UpdateViewMatrix();
	collisionSystem.Update(deltaTime);
	physicsSystem.Update(deltaTime);
	triggerSystem.Update(deltaTime);

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

		// Then render player with proper depth testing
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);  // Only render if closer than existing geometry
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		for (auto opaqueActor : opaqueActors) {
			glUseProgram(opaqueActor->GetComponent<ShaderComponent>()->GetProgram());
			//glUniform1f(opaqueActor->GetComponent<ShaderComponent>()->GetUniformID("index"), animIndex);
			glUniformMatrix4fv(opaqueActor->GetComponent<ShaderComponent>()->GetUniformID("modelMatrix"), 1, GL_FALSE, opaqueActor->GetModelMatrix());
			glBindTexture(GL_TEXTURE_2D, opaqueActor->GetComponent<MaterialComponent>()->getTextureID());
			opaqueActor->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
		}

		RenderAdrielMagik();

		for (auto transparentActor : transparentActors) {
			glUseProgram(transparentActor->GetComponent<ShaderComponent>()->GetProgram());
			glUniform1i(transparentActor->GetComponent<ShaderComponent>()->GetUniformID("NPC_id"), transparentActor->NPCid);
			glUniform1i(transparentActor->GetComponent<ShaderComponent>()->GetUniformID("talking"), true);
			glUniform2f(transparentActor->GetComponent<ShaderComponent>()->GetUniformID("playeranimIndex"), animIndex, animIndex);
			glUniform2f(transparentActor->GetComponent<ShaderComponent>()->GetUniformID("NPCanimIndex"), NPCanimIndex, 0);

			glUniformMatrix4fv(transparentActor->GetComponent<ShaderComponent>()->GetUniformID("modelMatrix"), 1, GL_FALSE, transparentActor->GetModelMatrix());
			if (transparentActor->GetComponent<MaterialComponent>()) {
				glBindTexture(GL_TEXTURE_2D, transparentActor->GetComponent<MaterialComponent>()->getTextureID());
				transparentActor->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
			}
		}
	
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



void CapstoneSceneDream::AdrielMagik() {
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

void CapstoneSceneDream::RenderAdrielMagik() const{
	for (auto actor : collisionSystem.collidingActors) {
		//if sphere
		CollisionComponent* collider = actor->GetComponent<CollisionComponent>().get();

		if (collider->GetColliderType() == ColliderType::AABB) {
			DrawCube(collider->GetAABB());
		}

		if (collider->GetColliderType() == ColliderType::Sphere) {
			DrawSphere(actor->GetComponent<TransformComponent>()->GetPosition(), collider->GetRadisu());
		}
	}
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