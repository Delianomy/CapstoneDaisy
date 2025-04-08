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

bool TriggerSystem::SphereAABBCollisionDetection(const Sphere& s, const AABB& aabb) const {
    //glm::vec2 aabb_half_extents(two.Size.x / 2.0f, two.Size.y / 2.0f);
    Vec3 aabb_half_extents = Vec3(aabb.rx, aabb.ry, aabb.rz);

    //glm::vec2 difference = center - aabb_center;
    Vec3 difference = s.center - aabb.center;

    Vec3 clamped;
    clamped.x = std::max(-aabb_half_extents.x, std::min(difference.x, aabb_half_extents.x));
    clamped.y = std::max(-aabb_half_extents.y, std::min(difference.y, aabb_half_extents.y));
    clamped.z = std::max(-aabb_half_extents.z, std::min(difference.z, aabb_half_extents.z));
    Vec3 closest = aabb.center + clamped;

    //retrieve vector between center circle and closest point AABB and check if length <= radius
    difference = closest - s.center;

    if (VMath::mag(difference) < s.r) {
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
            
            //Initializing the types REEEEE
            AABB B1, B2;
            Sphere S1, S2;

            bool collided = false;

            switch (TriggerCompA->colliderType) {
                case TriggerType::Box:
                    B1 = AABB(PhysicsCompA->GetPosition(), Vec3(TriggerCompA->box.rx, TriggerCompA->box.ry, TriggerCompA->box.rz));

                    switch (TriggerCompB->colliderType) {
                        case TriggerType::Sphere:
                            S2 = Sphere(PhysicsCompB->GetPosition(), TriggerCompB->radius);
                            collided = SphereAABBCollisionDetection(S2, B1);
                            break;
                    }
                    break;

                case TriggerType::Sphere:
                    S1 = Sphere(PhysicsCompA->GetPosition(), TriggerCompA->radius);

                    switch (TriggerCompB->colliderType) {
                        case TriggerType::Box:
                            B2 = AABB(PhysicsCompB->GetPosition(), Vec3(TriggerCompB->box.rx, TriggerCompB->box.ry, TriggerCompB->box.rz));
                            collided = SphereAABBCollisionDetection(S1, B2);
                            break;

                        case TriggerType::Sphere:
                            S2 = Sphere(PhysicsCompB->GetPosition(), TriggerCompB->radius);
                            collided = SphereSphereCollisionDetection(S1, S2);
                            break;
                     
                    }
                    break;
            }

            if (collided) {
                TriggerCompA->Call(triggeringActors[j]);
                TriggerCompB->Call(triggeringActors[i]);
            }
        }
    }
}