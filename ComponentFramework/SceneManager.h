#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <string>
#include "Window.h"
#include "Inventory.h"

class CameraActor;


enum class QuestState { NotStarted, InProgress, Completed };

struct Quest {
	Quest() {}
	Quest(int id_, std::string name_, QuestState state_) {
		id = id_;
		name = name_;
		state = state_;
	};
	int id;
	std::string name;
	QuestState state = QuestState::NotStarted;

	// Optional callbacks
	std::function<void()> OnStart;
	std::function<void()> OnComplete;
};


class QuestManager {
	

public:
	std::unordered_map<int, Quest> quests;
	void AddQuest(const Quest& quest) {
		quests[quest.id] = quest;
	}

	void StartQuest(int id) {
		if (quests[id].state == QuestState::NotStarted) {
			quests[id].state = QuestState::InProgress;
			if (quests[id].OnStart) quests[id].OnStart();
		}
	}

	void CompleteQuest(int id) {
		if (quests[id].state == QuestState::InProgress) {
			quests[id].state = QuestState::Completed;
			if (quests[id].OnComplete) quests[id].OnComplete();
		}
	}

	QuestState GetState(int id) {
		return quests[id].state;
	}

	bool IsActive(int id) {
		return quests[id].state == QuestState::InProgress;
	}
};

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
	class Timer* timer;
	class Window* window;

	unsigned int fps;
	bool isRunning;
	bool fullScreen;
	void BuildNewScene(SCENE_NUMBER scene_);
public:
	QuestManager* questManager;
	Quest currentQuest;
	void CreateQuests();
	int getWindowHeight() { return window->getHeight(); };
	int getWindowWidth() { return window->getWidth(); };
	void AddItemToInventory(std::shared_ptr<Actor> other, int index);
	void DropItemFromInventory(int index);
	Vec2 WorldToScreenCoordinates(Vec3 coords, CameraActor* camera);
};


#endif SCENEMANAGER_H