#include "Inventory.h"

void Inventory::AddItem(Ref<PickableItem> item) {
	//Check if the item is already in the inventory
	for (int i = 0; i < inventorySize; i++) {
		if (items[i] == item) return;
	}

	for (int i = 0; i < inventorySize; i++) {
		//Check if there is free space
		if (items[i] == nullptr) {
			AddItem(item, i);
			return;
		}

	}
}

void Inventory::AddItem(Ref<PickableItem> item, int index) {
	if (index >= inventorySize || index < 0) {
		std::cout << "INVENTORY: Attempted to add an item with a non valid index";
		return;
	}

	items[index] = item;
}

std::string Inventory::ToString() {
	std::string result = "";
	for (int i = 0; i < inventorySize; i++) {
		if (items[i] == nullptr) { continue; }
		result += items[i]->name;
		result += "\n";
	}
	return result;
}

