#ifndef SANDBOX_ADRIEL_H
#define SANDBOX_ADRIEL_H
#include "Scene.h"
#include "Vector.h"
#include "Matrix.h"
#include "Component.h"
#include "AssetManager.h"
#include "PhysicsSystem.h"
#include "CollisionSystem.h"
#include "TriggerSystem.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include "SceneManager.h"
#include "Inventory.h"
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class CameraActor;
class LightActor;
class Actor;
class SkyBox;

#include "Scene.h"
class SandboxAdriel : public Scene 
{
	Ref<AssetManager> assetManager;
	Ref<CameraActor> camera;
	Ref<LightActor> light;

	Ref<Actor> player;
	Ref<Actor> cube;
	Ref<Actor> skybox;

	//Middle part
	Ref<Actor> RightIsland;
	Ref<Actor> MiddleIsland;
	Ref<Actor> LeftIsland;
	Ref<Actor> House;
	Ref<Actor> Flower_One;


	//Test collisions
	Ref<Actor> sphere;
	Ref<Actor> plane;

	//Systems
	PhysicsSystem physicsSystem;
	CollisionSystem collisionSystem;
	TriggerSystem triggerSystem;
	SceneManager* sceneManagerRef;

	//Debug Meshes
	Ref<Actor> DebugSphere;
	Ref<Actor> DebugCube;

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

	//Inventory system
	Inventory* inventory;

	int animIndex = 0;
	float currentTime = 0.0f;
	float frameSpeed = 0.1f;
	bool drawNormals;
	bool drawOverlay;

public:
	Matrix4 orient;
	explicit SandboxAdriel(SceneManager* scenemanager);
	bool CreateLevelLayout();
	virtual ~SandboxAdriel();

	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime);
	virtual void Render() const;
	virtual void HandleEvents(const SDL_Event& sdlEvent);

	Plane CalculatePlaneCollider(Ref<Actor> obj);
		void DrawNormals(const Vec4 color) const;

	void DrawMeshOverlay(const Vec4 color) const;
	void DrawUI_imgui();

	/// Adriel Territory
	void PrintStatement(std::shared_ptr<Actor> a) {
		std::cout << "I collided with ";
		std::cout << a.get() << "\n";
	}
	/// <summary>
	/// Draws a sphere actor that is in wireframe mode. Use this in the Render() only. Kind expensive, so don't use it as much
	/// </summary>
	/// <param name="pos">Position of the Sphere</param>
	/// <param name="radius">How big the sphere is</param>
	void DrawSphere(Vec3 pos, float radius) const;
	void DrawSphere(Sphere s) const;
	/// <summary>
	/// Draws a cube actor that is in wireframe mode. Use this in Render() only. Kind of expensive, so don't use it as much
	/// </summary>
	/// <param name="pos">Position of the cube</param>
	/// <param name="radius">width, height, depth of the cube</param>
	void DrawCube(Vec3 pos, Vec3 dimensions) const;
	void DrawCube(AABB a) const;
	void DebugUI();

	void TryAddItem(std::shared_ptr<Actor> other);
};

#endif // CAPSTONESCENE_DREAM_H
