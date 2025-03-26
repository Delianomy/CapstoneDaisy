#include "PickableItem.h"
#include "MaterialComponent.h"
#include "ShaderComponent.h"
#include "TriggerComponent.h"
#include "AssetManager.h"
#include "PhysicsComponent.h"
#include "MeshComponent.h"

PickableItem::PickableItem(Ref<AssetManager> assMan, std::string name_, Vec3 pos, float triggerRadius, Vec3 scale, std::shared_ptr<MaterialComponent> material) :
	InteractableActor(assMan, pos,triggerRadius, scale, material) {
	name = name_;
	locked = true;
}
