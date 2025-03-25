#pragma once
#include "InteractableActor.h"

class InteractionManager{
public:
	InteractionManager() {}
	void Reset() { current = nullptr; }
	void SetCurrentInteraction(Ref<InteractableActor> curr) { current = curr; }
	Ref<InteractableActor> GetCurrentInteraction() const { return current; }

private:
	Ref<InteractableActor> current = nullptr;

};

