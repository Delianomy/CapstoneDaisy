#include "PickableItem.h"
#include "MaterialComponent.h"
#include "ShaderComponent.h"
#include "TriggerComponent.h"
#include "AssetManager.h"
#include "PhysicsComponent.h"
#include "ShaderComponent.h"
#include "MeshComponent.h"

PickableItem::PickableItem(Ref<AssetManager> assMan, std::string name_, Vec3 pos, float triggerRadius, Vec3 scale, std::shared_ptr<MaterialComponent> material, std::shared_ptr<ShaderComponent> shader) :
	InteractableActor(assMan, pos,triggerRadius, scale, material, shader) {
	name = name_;
	locked = true;
}
