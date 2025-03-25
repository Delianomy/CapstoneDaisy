#pragma once
#include "PickableItem.h"
#include <functional>
#include <sstream>
class Inventory{
public:
	//Temporary variable for when items are selected and being added to the inventory
	//It's an actor because we still need to perform some operations in the scene
	//Afterwards do we cast it into a PickableItem
	Ref<PickableItem> pendingItem;

	//Inventory array
	static const int inventorySize = 3;
	Ref<PickableItem> items[inventorySize];
	
	//Initialize everything as nullptr
	Inventory() { 
		for (int i = 0; i < inventorySize; i++) {
			items[i] = nullptr;
		}
	};
	~Inventory() {};

	/// <summary>
	/// Attempts to find a free slot in the 'items' array and add the item to the array
	/// </summary>
	/// <param name="item">Pointer to the item</param>
	void AddItem(Ref<PickableItem> item);

	/// <summary>
	/// Add an item in a specific index
	/// </summary>
	/// <param name="item">Pointer to the item</param>
	/// <param name="index">Index to add it to</param>
	void AddItem(Ref<PickableItem> item, int index);

	void AddPendingItem(int index);

	/// <summary>
	/// Prints the items inside of the inventory
	/// </summary>
	/// <returns></returns>
	std::string ToString();
};

