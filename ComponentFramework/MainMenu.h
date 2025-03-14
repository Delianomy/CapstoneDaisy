#ifndef CAPSTONESCENE_MAIN_MENU_H
#define CAPSTONESCENE_MAIN_MENU_H
#include "Scene.h"
#include "Vector.h"
#include "Matrix.h"
#include "Component.h"
#include "AssetManager.h"
#include "PhysicsSystem.h"
#include "CollisionSystem.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include "AudioSystem.h"
#include "SceneManager.h"
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class CameraActor;
class LightActor;
class Actor;
class SkyBox;
class Room;

#include "Scene.h"
class MainMenu : public Scene
{
	Ref<AssetManager> assetManager;
	Ref<CameraActor> camera;
	Ref<LightActor> light;
	Ref<Actor> player;
	Ref<Actor> cube;
	Ref<Actor> skybox;
	Ref<Actor> room;

	//Middle part
	Ref<Actor> RightIsland;
	Ref<Actor> MiddleIsland;
	Ref<Actor> LeftIsland;
	Ref<Actor> House;
	Ref<Actor> Flower_One;


	//Test collisions
	Ref<Actor> sphere;
	Ref<Actor> plane;


	PhysicsSystem physicsSystem;
	CollisionSystem collisionSystem;
	SceneManager* sceneManagerRef;
	float playerAngle;
	float iTime = 0.0f;

	//player controls imput
	bool goLeft = false;
	bool goRight = false;
	bool goForward = false;
	bool goBackwards = false;
	bool isMovable = false;
	bool rotatePlayerRight = false;
	bool rotatePlayerLeft = false;
	Vec3 forwardVector = Vec3(0.0f, 0.0f, 0.1f);
	Vec3 lefr_right_Vector = Vec3(0.1f, 0.0f, 0.0f);
	float walkSpeed = 40.0f;


	//invetory UI imput
	bool inventoryButtonPressed = true;
	bool select_item_1 = false;
	bool select_item_2 = false;
	bool select_item_3 = false;



	int animIndex = 0;
	float currentTime = 0.0f;
	float frameSpeed = 0.1f;
	bool drawNormals;
	bool drawOverlay;

	AudioManager audioManager;
public:
	Matrix4 orient;
	explicit MainMenu(SceneManager* scenemanager);
	virtual ~MainMenu();

	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime);
	virtual void Render() const;
	virtual void HandleEvents(const SDL_Event& sdlEvent);


	/// The w component holds the scale of the normal
	void DrawUI_imgui();
};

#endif // CAPSTONESCENE_MAIN_MENU_H
