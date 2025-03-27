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
	DialogueSystem dialogueSystem;

	Ref<AssetManager> assetManager;
	Ref<CameraActor> camera;
	Ref<LightActor> light;
	Ref<Actor> player;
	Ref<Actor> doll;
	Ref<Actor> books;
	Ref<Actor> room;
	//Ref<Actor> doll;
	float playerAngle;
	
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
public:
	Matrix4 orient;
	explicit CapstoneScene();
	virtual ~CapstoneScene();

	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime);
	int Pick(int x, int y);
	virtual void Render() const;
	virtual void HandleEvents(const SDL_Event& sdlEvent);

	
	/// The w component holds the scale of the normal
	void DrawNormals(const Vec4 color) const;

	void DrawMeshOverlay(const Vec4 color) const;
	void InitializeDialogue();
};

#endif // CAPSTONESCENE_H
