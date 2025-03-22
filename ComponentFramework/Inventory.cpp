#include "Inventory.h"

void Inventory::AddItem(PickableItem* item) {
	for (int i = 0; i < inventorySize; i++) {
		if (items[i] != nullptr) {
			AddItem(item, i);
		}
	}
}

void Inventory::AddItem(PickableItem* item, int index) {
	if (index >= inventorySize || index < 0) {
		std::cout << "INVENTORY: Attempted to add an item with a non valid index";
		return;
	}

	if (items[index] != nullptr) {
		items[index] = item;
	}
	else {
		DropCallback(item);
	}
}

void Inventory::PrintInventory() {
	std::cout << "Inventory items: \n";
	for (int i = 0; i < inventorySize; i++) {
		std::cout << items[i]->name << "\n";
	}
}