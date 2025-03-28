#ifndef INTERACTABLE_ACTOR_H
#define INTERACTABLE_ACTOR_H

#include "Actor.h"
#include <functional>

class AssetManager;
class MaterialComponent;


class InteractableActor : public Actor {
private:

	//Callback for when the actor is interacted
	std::function<void()> OnInteract;

public: 
	//If true, the actor cannot be interacted with using the interact button 
	bool locked = false;

	InteractableActor(Ref<AssetManager> assMan, Vec3 pos, float triggerRadius = 0.5f, Vec3 scale = Vec3(0.1f, 0.1f, 0.1f), std::shared_ptr<MaterialComponent> material = nullptr);

	void Bind(std::function<void()> func) { OnInteract = func;}
	void Invoke() { if (!locked) { OnInteract(); } }
};

#endif