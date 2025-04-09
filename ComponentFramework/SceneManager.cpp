#include <SDL.h>
#include "SceneManager.h"
#include "Timer.h"
#include "Window.h"
#include "CapstoneScene.h"
#include "CapstoneSceneDream.h"
#include "SandboxAdriel.h"
#include "CameraActor.h"
#include "ShaderTestScene.h"
#include "MainMenu.h"



SceneManager::SceneManager():
	currentScene(nullptr), window(nullptr), timer(nullptr),
	fps(60), isRunning(false), fullScreen(false) {
	Debug::Info("Starting the SceneManager", __FILE__, __LINE__);
}

SceneManager::~SceneManager() {
	if (currentScene) {
		delete currentScene;
		currentScene = nullptr;
	}
	//hi
	if (timer) {
		delete timer;
		timer = nullptr;
	}

	if (window) {
		delete window;
		window = nullptr;
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	Debug::Info("Deleting the SceneManager", __FILE__, __LINE__);
}

bool SceneManager::Initialize(std::string name_, int width_, int height_) {

	window = new Window();
	if (!window->OnCreate(name_, width_, height_)) {
		Debug::FatalError("Failed to initialize Window object", __FILE__, __LINE__);
		return false;
	}

	timer = new Timer();
	if (timer == nullptr) {
		Debug::FatalError("Failed to initialize Timer object", __FILE__, __LINE__);
		return false;
	}

	questManager = new QuestManager();
	CreateQuests();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(window->getWindow(), window->getContext());
	ImGui_ImplOpenGL3_Init();


	customFont= io.Fonts->AddFontFromFileTTF("fonts/lunchds.ttf", 20.0f);

	/********************************   Default first scene   ***********************/
	BuildNewScene(SCENE_NUMBER::SCENE_CAPSTONE_DREAM);


	return true;
}


/// This is the whole game
void SceneManager::Run() {
	timer->Start();
	isRunning = true;
	while (isRunning) {
		timer->UpdateFrameTicks();
		currentScene->Update(timer->GetDeltaTime());
		currentScene->Render();
		SDL_GL_SwapWindow(window->getWindow());
		HandleEvents();
		SDL_Delay(timer->GetSleepTime(fps));
	}
}

void SceneManager::HandleEvents() {
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent)) {
		if (sdlEvent.type == SDL_EventType::SDL_QUIT) {
			isRunning = false;
			return;
		}
		else if (sdlEvent.type == SDL_KEYDOWN) {
			switch (sdlEvent.key.keysym.scancode) {
			case SDL_SCANCODE_ESCAPE:
				isRunning = false;
				return;
				[[fallthrough]]; /// C17 Prevents switch/case fallthrough warnings

			case SDL_SCANCODE_F1:

				//currentScene = bedroomScene;
				BuildNewScene(SCENE_NUMBER::SCENE_CAPSTONE);
				break;

			case SDL_SCANCODE_F2:
				//currentScene = dreamScene;
				BuildNewScene(SCENE_NUMBER::SCENE_CAPSTONE_DREAM);
				break;
			case SDL_SCANCODE_F3:
				BuildNewScene(SCENE_NUMBER::SCENE_SHADER_TEST);
				break;
			case SDL_SCANCODE_F4:
				BuildNewScene(SCENE_NUMBER::SCENE_MAIN_MENU);
				break;
			case SDL_SCANCODE_F11:
				BuildNewScene(SCENE_NUMBER::SCENE0);
				break;
			}
		}
		if (currentScene == nullptr) {
			Debug::FatalError("Failed to initialize Scene", __FILE__, __LINE__);
			isRunning = false;
			return;
		}
		currentScene->HandleEvents(sdlEvent);
	}
}

void SceneManager::BuildNewScene(SCENE_NUMBER scene) {
	bool status;

	if (currentScene != nullptr) {
		currentScene->OnDestroy();
		delete currentScene;
		currentScene = nullptr;
	}

	switch (scene) {

	case SCENE_NUMBER::SCENE_CAPSTONE:
		currentScene = new CapstoneScene();
		currentScene->SetSceneManager(this);
		status = currentScene->OnCreate();
		break;
	case SCENE_NUMBER::SCENE_CAPSTONE_DREAM:
		currentScene = new CapstoneSceneDream(this);
		currentScene->SetSceneManager(this);
		status = currentScene->OnCreate();
		break;
	case SCENE_NUMBER::SCENE_SHADER_TEST:
		currentScene = new ShaderTestScene();
		status = currentScene->OnCreate();
		break;
	case SCENE_NUMBER::SCENE_MAIN_MENU:
		currentScene = new MainMenu(this);
		status = currentScene->OnCreate();
		break;
	case SCENE_NUMBER::SCENE_SANDBOX_ADRIEL:
		currentScene = new SandboxAdriel(this);
		status = currentScene->OnCreate();
		break;

	default:
		Debug::Error("Incorrect scene number assigned in the manager", __FILE__, __LINE__);
		currentScene = nullptr;
		break;
	}
}

void SceneManager::CreateQuests()
{
	/// Start quest by talking to owl
	questManager->AddQuest(Quest(0, "Bring The Bear", QuestState::NotStarted));
	questManager->AddQuest(Quest(1, "Talk to Fairy", QuestState::NotStarted));
	questManager->AddQuest(Quest(2, "Bring the enchanted teddy", QuestState::NotStarted));
}

void SceneManager::AddItemToInventory(std::shared_ptr<Actor> other, int index)
{
	//Check if it's a pickable item
	if (std::dynamic_pointer_cast<PickableItem>(other) != nullptr) {
		//Drops the item
		if (inventory.items[index] != nullptr) {
			DropItemFromInventory(index);
		}

		//Add the item to the inventory
		inventory.AddItem(std::dynamic_pointer_cast<PickableItem>(other), index);

		//Remove the item from the triggerSystem
		int index = 0;
		for (int i = 0; i < currentScene->triggerSystem.triggeringActors.size(); i++) {
			if (currentScene->triggerSystem.triggeringActors[i] == other) {
				index = i;
				break;
			}
		}
		currentScene->triggerSystem.triggeringActors.erase(currentScene->triggerSystem.triggeringActors.begin() + index);

		//Remove the item from the opaque actors
		index = 0;
		for (int i = 0; i < currentScene->transparentActors.size(); i++) {
			if (currentScene->transparentActors[i] == other) {
				index = i;
				break;
			}
		}
		currentScene->transparentActors.erase(currentScene->transparentActors.begin() + index);
	}
}

void SceneManager::DropItemFromInventory(int index) {
	//Add the object back into the actor vectors
	currentScene->transparentActors.push_back(inventory.items[index]);
	currentScene->triggerSystem.AddActor(inventory.items[index]);
	//Remove the item from the inventory
	inventory.RemoveItem(index);
}

Vec2 SceneManager::WorldToScreenCoordinates(Vec3 coords, CameraActor* camera){
	Vec4 screenPos = camera->GetProjectionMatrix() * camera->GetViewMatrix() * Vec4(coords, 1.0f);
	Vec4 NDC = screenPos * (1 / screenPos.w);
	Vec2 screenSpaceCoords = Vec2((NDC.x + 1) / 2 * getWindowWidth(), (1 - NDC.y) / 2 * getWindowHeight());
	return screenSpaceCoords;
}



