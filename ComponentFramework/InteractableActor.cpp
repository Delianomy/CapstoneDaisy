#include "InteractableActor.h"
#include "AssetManager.h"
#include "MaterialComponent.h"
#include "TriggerComponent.h"
#include "ShaderComponent.h"
#include "MeshComponent.h"

InteractableActor::InteractableActor(Ref<AssetManager> assMan, Vec3 pos, float triggerRadius, std::shared_ptr<MaterialComponent> material) : Actor(nullptr) {
	AddComponent<TransformComponent>(this, pos, Quaternion());
	AddComponent<ShaderComponent>(assMan->GetComponent<ShaderComponent>("TextureShader"));
	AddComponent<TriggerComponent>(this, triggerRadius);
	AddComponent<MeshComponent>(assMan->GetComponent<MeshComponent>("Plane"));

	if (material != nullptr) { AddComponent<MaterialComponent>(material); }
	else { AddComponent<MaterialComponent>(assMan->GetComponent<MaterialComponent>("ChessBoard")); }

}

