#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <string>
#include "Window.h"
#include <SDL_mixer.h>

//#include "fmod.h"
#include "fmod.hpp"



class SceneManager  {
public:
	FMOD_RESULT result;
	FMOD::System* pSystem = nullptr;
	


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
		SCENE_SHADER_TEST

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