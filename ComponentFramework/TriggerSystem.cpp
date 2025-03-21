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

void TriggerSystem::SphereSphereCollisionResponse(Sphere s1, Ref<PhysicsComponent> pc1, Sphere s2, Ref<PhysicsComponent> pc2)
{
    float e = 1.0f; /// coefficient of restitution
    Vec3 L = s1.center - s2.center;
    Vec3 n = VMath::normalize(L);
    Vec3 v1 = pc1->vel;
    Vec3 v2 = pc2->vel;
    float m1 = pc1->mass;
    float m2 = pc2->mass;

    float v1p = VMath::dot(v1, n);
    float v2p = VMath::dot(v2, n);

    if (v1p - v2p > 0.0f) { /// The colliding objects are not yet free from one and another, come back next cycle
        return;
    }
    float v1p_new = ((m1 - e * m2) * v1p) + ((1.0f + e) * m2 * v2p) / (m1 + m2);
    float v2p_new = ((m2 - e * m1) * v2p) + ((1.0f + e) * m1 * v1p) / (m1 + m2);

    if (!pc1->isStatic) {
        pc1->vel = v1 + (v1p_new - v1p) * n;
    }
    if (!pc2->isStatic) {
        pc2->vel = v2 + (v2p_new - v2p) * n;
    }
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
                Ref<PhysicsComponent> pc1 = triggeringActors[i]->GetComponent<PhysicsComponent>();
                Ref<PhysicsComponent> pc2 = triggeringActors[j]->GetComponent<PhysicsComponent>();
                std::cout << "TRIGGERED"<<std::endl;
                SphereSphereCollisionResponse(s1, pc1, s2, pc2);
            }

        }


    }

}