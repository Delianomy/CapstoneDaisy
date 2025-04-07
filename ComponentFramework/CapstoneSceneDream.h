#ifndef CAPSTONESCENE_DREAM_H
#define CAPSTONESCENE_DREAM_H
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
#include <algorithm>
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class CameraActor;
class LightActor;
class Actor;
class SkyBox;

#include "Scene.h"

enum class PlayerAnimType {
	Idle = 0,
	Walking,
	Jumping,
	SwimmingIdle,
	Swimming
};

struct AnimationInfo {
	int row;       // Sprite sheet row (Y index)
	int frameCount; // Number of frames in that row
};

class CapstoneSceneDream : public Scene 
{
	Ref<AssetManager> assetManager;
	Ref<CameraActor> camera;
	Ref<LightActor> light;


	Ref<Actor> player;
	Ref<Actor> cube;
	Ref<Actor> skybox;
	Ref<Actor> mermaid;
	Ref<Actor> mrOwl;
	Ref<Actor> mrsMouse;
	Ref<Actor> fairy;

	//Middle part
	Ref<Actor> Island1;
	Ref<Actor> Island2;
	Ref<Actor> Island3;
	Ref<Actor> House;
	Ref<Actor> Flower_1;
	Ref<Actor> Flower_2;
	Ref<Actor> Flower_3;
	Ref<Actor> Flower_4;
	Ref<Actor> Beanstalk1;
	Ref<Actor> Leaf1;
	Ref<Actor> Leaf2;
	Ref<Actor> Flower_5; 
	Ref<Actor> Flower_6;
	Ref<Actor> Tower1;
	Ref<Actor> Bridge;
	Ref<Actor> BottomOfTheOcean;
	Ref<Actor> Bush;
	


	//Test collisions
	Ref<Actor> sphere;
	Ref<Actor> plane;


	PhysicsSystem physicsSystem;
	CollisionSystem collisionSystem;
	TriggerSystem triggerSystem;
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

	//NewInput
	Vec3 movementInput = Vec3();
	bool playerIsGrounded = false;

	//invetory UI imput
	bool inventoryButtonPressed = true;
	bool select_item_1 = false;
	bool select_item_2 = false;
	bool select_item_3 = false;

	//Inventory system
	Inventory* inventory;

	//Debug Meshes
	Ref<Actor> DebugSphere;
	Ref<Actor> DebugCube;
	void AdrielMagik();
	void RenderAdrielMagik() const;

	int NPCanimIndex = 0;
	float currentTime = 0.0f;
	float NPCcurrentTime = 0.0f;
	float frameSpeed = 0.1f;
	bool drawNormals;
	bool drawOverlay;




	bool playerIsSwimming = false;
	PlayerAnimType currentAnim;
	Vec2 animIndex;

	std::unordered_map<PlayerAnimType, AnimationInfo> animTable = {
	{PlayerAnimType::Idle,         {5, 11}},
	{PlayerAnimType::Walking,      {4, 13}},
	{PlayerAnimType::Jumping,      {3, 13}},
	{PlayerAnimType::SwimmingIdle, {2, 6}},
	{PlayerAnimType::Swimming,     {1, 10}}
	};
public:
	Matrix4 orient;
	explicit CapstoneSceneDream(SceneManager* scenemanager);
	bool CreateLevelLayout();
	virtual ~CapstoneSceneDream();

	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime);
	virtual void Render() const;
	virtual void HandleEvents(const SDL_Event& sdlEvent);

	
	/// The w component holds the scale of the normal
	void DrawNormals(const Vec4 color) const;

	void DrawMeshOverlay(const Vec4 color) const;
	void DrawUI_imgui();
	
	///Adriel's land
	void DrawSphere(Vec3 pos, float radius) const;
	void DrawSphere(Sphere s) const;
	void DrawCube(Vec3 pos, Vec3 dimensions) const;
	void DrawCube(AABB a) const;
	void DrawRay(Ray ray) const;
	void PlayerGroundCheck();
	void RenderColliders() const;

	Vec2 GetAnimIndex(float deltaTime, float& currentTime, PlayerAnimType animType, float frameSpeed) {

		auto& anim = animTable[animType];
		int xIndex = static_cast<int>(currentTime / frameSpeed) % anim.frameCount;
		int yIndex = anim.row;
		return Vec2(static_cast<float>(xIndex), static_cast<float>(yIndex));
	}
};

#endif // CAPSTONESCENE_DREAM_H
