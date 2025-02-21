#pragma once
#include <vector>
#include "PhysicsComponent.h"
#include "Actor.h"
#include "Debug.h"

class PhysicsSystem {
private:
	const float gravity = -9.81f;
	std::vector<Ref<Actor>> physicsActors;

public:

	void AddActor(Ref<Actor> actor_) {
		physicsActors.push_back(actor_);
	}

	Ref<PhysicsComponent> GetActor() const {
		for (auto component : physicsActors) {
			if (dynamic_cast<PhysicsComponent*>(component.get())) {
				/// This is a dynamic cast designed for shared_ptr's
				/// https://en.cppreference.com/w/cpp/memory/shared_ptr/pointer_cast
				return std::dynamic_pointer_cast<PhysicsComponent>(component);
			}
		}
		return Ref<PhysicsComponent>(nullptr);
	}


	void Update(const float deltaTime);
};

