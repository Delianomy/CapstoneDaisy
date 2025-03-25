#include "TriggerSystem.h"
#include <cmath>
#include <algorithm>

bool TriggerSystem::SphereSphereCollisionDetection(const Sphere& s1, const Sphere& s2) const
{
    float distance = VMath::distance(s1.center, s2.center);
    if (distance < s1.r + s2.r) {
        return true;
    }
    return false;
}



void TriggerSystem::Update(const float deltaTime)
{
    for (size_t i = 0; i < triggeringActors.size(); ++i) {
        for (size_t j = i + 1; j < triggeringActors.size(); ++j) {
            if (triggeringActors[i] == triggeringActors[j]) { continue; }

            //Getting the components
            //First actor
            Ref<TriggerComponent> TriggerCompA = triggeringActors[i]->GetComponent<TriggerComponent>();
            Ref<TransformComponent> PhysicsCompA = triggeringActors[i]->GetComponent<TransformComponent>();

            //Second actor
            Ref<TriggerComponent> TriggerCompB = triggeringActors[j]->GetComponent<TriggerComponent>();
            Ref<TransformComponent> PhysicsCompB = triggeringActors[j]->GetComponent<TransformComponent>();

            Sphere S1 = Sphere(PhysicsCompA->GetPosition(), TriggerCompA->radius);
            Sphere S2 = Sphere(PhysicsCompB->GetPosition(), TriggerCompB->radius);

            if (SphereSphereCollisionDetection(S1, S2) == true) {
                TriggerCompA->Call(triggeringActors[j]);
                TriggerCompB->Call(triggeringActors[i]);
            }
        }
    }
}