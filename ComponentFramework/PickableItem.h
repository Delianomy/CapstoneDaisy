#ifndef PICKABLE_H
#define PICKABLE_H


#include <string> 
#include "Actor.h"
class MaterialComponent;
class AssetManager;
class TriggerCallback;;
class PickableItem : public Actor{
public:
	PickableItem(Ref<AssetManager> assMan, std::string name_, Vec3 pos, float triggerRadius = 0.5f, std::shared_ptr<MaterialComponent> material = nullptr);
	~PickableItem() {}

	void SetCallback(TriggerCallback* callback);
	std::string name;
};


#endif 