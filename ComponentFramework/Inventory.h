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

	/// <summary>
	/// A pending item is an item that is about to be added to the inventory but not yet.
	/// That item is temporarily stored in the 'pendingItem' variable
	/// </summary>
	/// <param name="index">Spot in the inventory you want to add it to</param>
	void AddPendingItem(int index);

	/// <summary>
	/// Removes an item from the inventory. Just sets the pointer to nullptr.
	/// Might cause a memory leak it you're not careful
	/// </summary>
	/// <param name="item">The item to remove</param>
	void RemoveItem(Ref<PickableItem> item);

	void RemoveItem(int index) { 
		if (index < 0 || index >= inventorySize) { return; }
		items[index] = nullptr; 
	}

	/// <summary>
	/// Prints the items inside of the inventory
	/// </summary>
	/// <returns></returns>
	std::string ToString();
};

