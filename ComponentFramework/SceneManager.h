#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <string>
#include "Window.h"
class SceneManager  {
public:
	
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

	class Scene* currentScene;
	class Scene* bedroomScene;
	class Scene* dreamScene;
	class Timer* timer;
	class Window* window;

	unsigned int fps;
	bool isRunning;
	bool fullScreen;
	void BuildNewScene(SCENE_NUMBER scene_);
public:
	int getWindowHeight() { return window->getHeight(); };
	int getWindowWidth() { return window->getWidth(); };
};


#endif // SCENEMANAGER_H