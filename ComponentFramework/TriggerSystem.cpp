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



            Sphere s1, s2;  /// I'm just going to do sphere-sphere collions first
            s1.r = triggeringActors[i]->GetComponent<TriggerComponent>()->radius;
            s1.center = triggeringActors[i]->GetComponent<PhysicsComponent>()->pos;

            s2.r = triggeringActors[j]->GetComponent<TriggerComponent>()->radius;
            s2.center = triggeringActors[j]->GetComponent<PhysicsComponent>()->pos;

            if (SphereSphereCollisionDetection(s1, s2) == true) {
                std::cout << "TRIGGERED" << std::endl;


            }


        }

    }
}