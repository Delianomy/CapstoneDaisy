#ifndef PICKABLE_H
#define PICKABLE_H

#include <string> 
#include "InteractableActor.h"

class MaterialComponent;
class AssetManager;
class TriggerCallback;

class PickableItem : public InteractableActor {
public:
	PickableItem(Ref<AssetManager> assMan, std::string name_, Vec3 pos, float triggerRadius = 0.5f, Vec3 scale = Vec3(0.1f, 0.1f, 0.1f), std::shared_ptr<MaterialComponent> material = nullptr);
	~PickableItem() {}
	std::string name;
};


#endif 