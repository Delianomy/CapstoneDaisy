#include "PickableItem.h"
#include "MaterialComponent.h"
#include "ShaderComponent.h"
#include "TriggerComponent.h"
#include "AssetManager.h"
#include "PhysicsComponent.h"

PickableItem::PickableItem(Ref<AssetManager> assMan, std::string name_, Vec3 pos, float triggerRadius, std::shared_ptr<MaterialComponent> material) : Actor(nullptr){
	AddComponent<PhysicsComponent>(this, pos, true,false );
	AddComponent<ShaderComponent>(assMan->GetComponent<ShaderComponent>("TextureShader"));
	AddComponent<TriggerComponent>(this, triggerRadius);

	if (material != nullptr) { AddComponent<MaterialComponent>(material); }
	else { AddComponent<MaterialComponent>(assMan->GetComponent<MaterialComponent>("ChessBoard"));}

	name = name_;
}

void PickableItem::SetCallback(TriggerCallback* callback) {
	GetComponent<TriggerComponent>()->SetCallback(callback);
}