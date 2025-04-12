#ifndef CAPSTONESCENE_H
#define CAPSTONESCENE_H
#include "Scene.h"
#include "Vector.h"
#include "Matrix.h"
#include "Component.h"
#include "AssetManager.h"

#include "DialogueSystem.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include <SDL_image.h>
#include "AudioSystem.h" 
#include "Inventory.h"
#include "PickableItem.h"
#include "SceneManager.h"


using namespace MATH;

/// Forward declarations 
union SDL_Event;
class CameraActor;
class LightActor;
class Actor;
class SkyBox;
class Room;
class DialogueSystem;

#include "Scene.h"
class CapstoneScene : public Scene 
{
	
	SDL_Surface* defaultCursorTexture;
	SDL_Surface* hoveredCursorTexture;
	SDL_Cursor* defaultCursor;
	SDL_Cursor* hoveredCursor;
	bool isHovering = false;
	bool hovering = false;
	int mouseX, mouseY;
	GLuint hoveredActorHiglightTexture;

		

	AssetManager* assetManager;
	Ref<CameraActor> camera;
	Ref<LightActor> light;
	Ref<Actor> player;


	Ref<AudioManager> audioManager;
	Ref<DialogueSystem> dialogueSystem;


	Ref<Actor> travelToDaisyLand;

	Ref<PickableItem> bear;
	Ref<PickableItem> moonTrinket;
	Ref<PickableItem> books;

	std::shared_ptr<PickableItem> bearInInventory;
	std::shared_ptr<PickableItem> booksInInventory;
	std::shared_ptr<PickableItem> moonInInventory;

	SceneManager* sceneManagerRef;
	Ref<Actor> room;
	float playerAngle;
	
	std::vector<std::vector<Dialogue>> dialogueSequences;
	std::vector<std::vector<Dialogue>> startDialogueSequence;

	bool goLeft = false;
	bool goRight = false;
	bool goForward = false;
	bool goBackwards = false;
	bool isMovable = false;
	bool rotatePlayerRight = false;
	bool rotatePlayerLeft = false;
	Vec3 forwardVector = Vec3(0.0f, 0.0f, 0.1f);
	Vec3 lefr_right_Vector = Vec3(0.1f, 0.0f, 0.0f);
	int animIndex = 0;
	float currentTime = 0.0f;
	float frameSpeed = 0.1f;
	bool drawNormals;
	bool drawOverlay;
	bool inventoryButtonPressed = true;

	bool rotateRight = false;
public:
	Matrix4 orient;
	explicit CapstoneScene(SceneManager* scenemanager);
	virtual ~CapstoneScene();

	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime);
	int Pick(int x, int y);
	virtual void Render() const;
	virtual void HandleEvents(const SDL_Event& sdlEvent);

	bool isCursoreOnObject(int x, int y);
	void InitializeDialogue();
	void DrawUI_imgui();
	void AdrielMagik();
	void StartGameDialogue();
};

#endif // CAPSTONESCENE_H
