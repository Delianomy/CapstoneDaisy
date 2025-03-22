#pragma once
#include "PickableItem.h"
#include <functional>
class Inventory{
private:
	static const int inventorySize = 3;
	PickableItem* items[inventorySize];

public:
	/// <summary>
	/// Callback event to when the item is dropped from the inventory
	/// </summary>
	std::function<void(PickableItem*)> DropCallback;

	Inventory() {};
	~Inventory() {};

	/// <summary>
	/// Attempts to find a free slot in the 'items' array and add the item to the array
	/// </summary>
	/// <param name="item">Pointer to the item</param>
	void AddItem(PickableItem* item);

	/// <summary>
	/// Add an item in a specific index
	/// </summary>
	/// <param name="item">Pointer to the item</param>
	/// <param name="index">Index to add it to</param>
	void AddItem(PickableItem* item, int index);


	/// <summary>
	/// Prints the items inside of the inventory
	/// </summary>
	/// <returns></returns>
	void PrintInventory();
};

