#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <string>
#include "Window.h"
#include "Inventory.h"

class CameraActor;
class SceneManager  {
public:
	Inventory inventory;
	SceneManager();
	~SceneManager();
	void Run();
	bool Initialize(std::string name_, int width_, int height_);
	void HandleEvents();
	
	
private:
	enum class SCENE_NUMBER {
		SCENE0 = 0,
		SCENE1,
		SCENE2,
		SCENE3,
		SCENE4,
		SCENE5,
		SCENE6,
		SCENE_CAPSTONE,
		SCENE_CAPSTONE_DREAM,
		SCENE_SHADER_TEST,
		SCENE_MAIN_MENU,
		SCENE_SANDBOX_ADRIEL

	};

	//Forward declarations
	class Scene* currentScene;
	class Scene* bedroomScene;
	class Scene* dreamScene;
	class Timer* timer;
	class Window* window;

	unsigned int fps;
	bool isRunning;
	bool fullScreen;
	void BuildNewScene(SCENE_NUMBER scene_);

	/// <summary>
	/// Loads the CapstoneScene and CapstoneDreamScene into memory
	/// </summary>
	/// <returns></returns>
	bool BuildCapstoneScenes();
public:
	int getWindowHeight() { return window->getHeight(); };
	int getWindowWidth() { return window->getWidth(); };
	void AddItemToInventory(std::shared_ptr<Actor> other, int index);
	void DropItemFromInventory(int index);
	Vec2 WorldToScreenCoordinates(Vec3 coords, CameraActor* camera);
};


#endif // SCENEMANAGER_H