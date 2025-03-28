#pragma once
#include "InteractableActor.h"
#include "PickableItem.h";
class ItemInteractable : public InteractableActor{
private:
    std::function<void()> OnCorrect = nullptr;
    std::function<void()> OnReject = nullptr;


public:
    ItemInteractable(Ref<AssetManager> assMan, std::string correctItemName, Vec3 pos, float triggerRadius = 0.5f, Vec3 scale = Vec3(0.1f, 0.1f, 0.1f), std::shared_ptr<MaterialComponent> material = nullptr);

    /// <summary>
    /// The name of the correct item that is accepted
    /// </summary>
    std::string correctItem;

    void BindToOnCorrect(std::function<void()> func) { OnCorrect = func; } //If the item is the correct one call this function
    void BindToOnReject(std::function<void()> func) { OnReject = func; } //If it's not call this one

    void InvokeOnCorrect() { OnCorrect(); }
    void InvokeOnReject() { OnReject(); }

    void TryItem(std::shared_ptr<PickableItem> item);
};

