#include "InteractableActor.h"
#include "AssetManager.h"
#include "MaterialComponent.h"
#include "TriggerComponent.h"
#include "ShaderComponent.h"
#include "MeshComponent.h"
#include "PhysicsComponent.h"

InteractableActor::InteractableActor(AssetManager* assMan, Vec3 pos, float triggerRadius, Vec3 scale, std::shared_ptr<MaterialComponent> material, std::shared_ptr<ShaderComponent> shader) : Actor(nullptr) {
	AddComponent<PhysicsComponent>(this, pos, Quaternion());
	AddComponent<TriggerComponent>(this, triggerRadius);
	AddComponent<MeshComponent>(assMan->GetComponent<MeshComponent>("Plane"));
	GetComponent<TransformComponent>()->SetScale(scale);

	if (shader != nullptr) { AddComponent<ShaderComponent>(shader); }
	else { AddComponent<ShaderComponent>(assMan->GetComponent<ShaderComponent>("SimpleTextureShader")); }

	if (material != nullptr) { AddComponent<MaterialComponent>(material); }
	else { AddComponent<MaterialComponent>(assMan->GetComponent<MaterialComponent>("ChessBoard")); }

}

