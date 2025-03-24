#include "PickableItem.h"
#include "MaterialComponent.h"
#include "ShaderComponent.h"
#include "TriggerComponent.h"
#include "AssetManager.h"
#include "PhysicsComponent.h"
#include "MeshComponent.h"

PickableItem::PickableItem(Ref<AssetManager> assMan, std::string name_, Vec3 pos, float triggerRadius, std::shared_ptr<MaterialComponent> material) : Actor(nullptr){
	AddComponent<PhysicsComponent>(this, pos, Vec3(0.1f, 0.1f, 0.1f), true, false);
	AddComponent<ShaderComponent>(assMan->GetComponent<ShaderComponent>("TextureShader"));
	AddComponent<TriggerComponent>(this, triggerRadius);
	AddComponent<MeshComponent>(assMan->GetComponent<MeshComponent>("Plane"));

	if (material != nullptr) { AddComponent<MaterialComponent>(material); }
	else { AddComponent<MaterialComponent>(assMan->GetComponent<MaterialComponent>("ChessBoard"));}

	name = name_;
}

void PickableItem::SetCallback(TriggerCallback* callback) {
	GetComponent<TriggerComponent>()->SetCallback(callback);
}