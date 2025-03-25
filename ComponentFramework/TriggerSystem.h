#ifndef TRIGGER_SYSTEM_H
#define TRIGGER_SYSTEM_H

#include <vector>
#include "TriggerComponent.h"
#include "PhysicsComponent.h"
#include "Actor.h"
#include "Debug.h"
#include <Sphere.h>
using namespace MATH;
using namespace MATHEX;


class TriggerSystem
{
private:

public:
	/// This function will check the the actor being added is new and has the all proper components 
	std::vector<Ref<Actor>> triggeringActors;
	void AddActor(Ref<Actor> actor_) {
		if (actor_->GetComponent<TriggerComponent>().get() == nullptr) {
			Debug::Error("The Actor must have a Trigger - ignored ", __FILE__, __LINE__);
			return;
		}
		if (actor_->GetComponent<TransformComponent>().get() == nullptr) {
			Debug::Error("The Actor must have a PhysicsComponent - ignored ", __FILE__, __LINE__);
			return;
		}

		triggeringActors.push_back(actor_);
	}

	bool SphereSphereCollisionDetection(const Sphere& s1, const Sphere& s2) const;
	
	void Update(const float deltaTime);


};

#endif // !TRIGGER_SYSTEM_H

