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
#include "ItemInteractable.h"
#include "InteractionManager.h"
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
	Ref<InteractionManager> interactionManager;
 	Ref<CameraActor> camera;
	Ref<LightActor> light;


	Ref<Actor> player;
	Ref<Actor> cube;
	Ref<Actor> skybox;
	Ref<Actor> mrsMouse;
	Ref<Actor> fairy;


	Ref<InteractableActor> mrOwl;
	Ref<InteractableActor> mermaid;



	Ref<Actor> Ocean;


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

	//NewInput
	Vec3 movementInput = Vec3();
	float walkSpeed = 10.0f;
	float jumpSpeed = 200.0f;
	float groudAccel = 20.0f;
	float airAccel = 3.0f;
	bool playerIsGrounded = false;
	Ray groundCheckRay = Ray(Vec3(), Vec3());

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

	//Debug messages
	std::string rayCollidedActors;

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

	//FRAMEBUFFER
	GLuint frameBuffer;
	GLuint textureColorbuffer;
	GLuint rbo;
	GLuint VAO;
	GLuint VBO;

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
	void CreateDebugMeshes();
	void DebugUI() const;



	void PendItemToInventory(std::shared_ptr<Actor> other);
	void AddItemToInventory(std::shared_ptr<Actor> other, int index);
	void DropItemFromInventory(int index);
	void PlayerTriggerCallback(Ref<Actor> other);


	Vec2 GetAnimIndex(float deltaTime, float& currentTime, PlayerAnimType animType, float frameSpeed) {

		auto& anim = animTable[animType];
		int xIndex = static_cast<int>(currentTime / frameSpeed) % anim.frameCount;
		int yIndex = anim.row;
		return Vec2(static_cast<float>(xIndex), static_cast<float>(yIndex));
	}
};

#endif // CAPSTONESCENE_DREAM_H
