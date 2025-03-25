#include "PickableItem.h"
#include "MaterialComponent.h"
#include "ShaderComponent.h"
#include "TriggerComponent.h"
#include "AssetManager.h"
#include "PhysicsComponent.h"
#include "MeshComponent.h"

PickableItem::PickableItem(Ref<AssetManager> assMan, std::string name_, Vec3 pos, float triggerRadius, std::shared_ptr<MaterialComponent> material) : 
	InteractableActor(assMan, pos, triggerRadius, material) {
	GetComponent<TransformComponent>()->SetScale(Vec3(0.1f, 0.1f, 0.1f));
	name = name_;
	locked = true;
}
