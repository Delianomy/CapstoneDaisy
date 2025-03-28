#include "ItemInteractable.h"

ItemInteractable::ItemInteractable(Ref<AssetManager> assMan, std::string correctItemName, Vec3 pos, float triggerRadius, Vec3 scale, std::shared_ptr<MaterialComponent> material) :
	InteractableActor(assMan, pos, triggerRadius, scale, material)
{
	correctItem = correctItemName;
}

void ItemInteractable::TryItem(std::shared_ptr<PickableItem> item) {
	if (item == nullptr) { return; }
	if (item->name == correctItem) {
		InvokeOnCorrect();
	}
	else {
		InvokeOnReject();
	}

}