#include "CollisionSystem.h"
#include <cmath>
#include <algorithm>

void CollisionSystem::Update(const float deltaTime) {

    for (size_t i = 0; i < collidingActors.size(); ++i) {
        for (size_t j = i + 1; j < collidingActors.size(); ++j) {

            if (collidingActors[i]->GetComponent<CollisionComponent>()->colliderType == ColliderType::Sphere &&
                collidingActors[j]->GetComponent<CollisionComponent>()->colliderType == ColliderType::Sphere) {

                Sphere s1, s2;  /// I'm just going to do sphere-sphere collions first
                s1.r = collidingActors[i]->GetComponent<CollisionComponent>()->radius;
                s1.center = collidingActors[i]->GetComponent<PhysicsComponent>()->pos;

                s2.r = collidingActors[j]->GetComponent<CollisionComponent>()->radius;
                s2.center = collidingActors[j]->GetComponent<PhysicsComponent>()->pos;

                if (SphereSphereCollisionDetection(s1, s2) == true) {
                    Ref<PhysicsComponent> pc1 = collidingActors[i]->GetComponent<PhysicsComponent>();
                    Ref<PhysicsComponent> pc2 = collidingActors[j]->GetComponent<PhysicsComponent>();
                    SphereSphereCollisionResponse(s1, pc1, s2, pc2);
                }

            }
            else if (collidingActors[i]->GetComponent<CollisionComponent>()->colliderType == ColliderType::Sphere &&
                collidingActors[j]->GetComponent<CollisionComponent>()->colliderType == ColliderType::AABB) {

                Sphere s;
                AABB box;

                s.r = collidingActors[i]->GetComponent<CollisionComponent>()->radius;
                s.center = collidingActors[i]->GetComponent<PhysicsComponent>()->pos;

                box.center = collidingActors[j]->GetComponent<CollisionComponent>()->aabb.center;
                box.rx = collidingActors[j]->GetComponent<CollisionComponent>()->aabb.rx;
                box.ry = collidingActors[j]->GetComponent<CollisionComponent>()->aabb.ry;
                box.rz = collidingActors[j]->GetComponent<CollisionComponent>()->aabb.rz;

                if (SphereAABBCollisionDetection(s, box)) {
                    std::cout << "Collided! " << std::endl;
                    Ref<PhysicsComponent> pc1 = collidingActors[i]->GetComponent<PhysicsComponent>();
                    Ref<PhysicsComponent> pc2 = collidingActors[j]->GetComponent<PhysicsComponent>();
                    SphereAABBCollisionResponce(s, pc1, box, pc2);
                }
             }
            else if (collidingActors[i]->GetComponent<CollisionComponent>()->colliderType == ColliderType::AABB &&
                collidingActors[j]->GetComponent<CollisionComponent>()->colliderType == ColliderType::Sphere) {

                Sphere s;
                AABB box;

                s.r = collidingActors[j]->GetComponent<CollisionComponent>()->radius;
                s.center = collidingActors[j]->GetComponent<PhysicsComponent>()->pos;

                box.center = collidingActors[i]->GetComponent<CollisionComponent>()->aabb.center;
                box.rx = collidingActors[i]->GetComponent<CollisionComponent>()->aabb.rx;
                box.ry = collidingActors[i]->GetComponent<CollisionComponent>()->aabb.ry;
                box.rz = collidingActors[i]->GetComponent<CollisionComponent>()->aabb.rz;

                if (SphereAABBCollisionDetection(s, box)) {
                    std::cout << "Collided! " << std::endl;
                    Ref<PhysicsComponent> pc1 = collidingActors[i]->GetComponent<PhysicsComponent>();
                    Ref<PhysicsComponent> pc2 = collidingActors[j]->GetComponent<PhysicsComponent>();
                    SphereAABBCollisionResponce(s, pc1, box, pc2);
                }
            }

             else if (collidingActors[i]->GetComponent<CollisionComponent>()->colliderType == ColliderType::AABB &&
                     collidingActors[j]->GetComponent<CollisionComponent>()->colliderType == ColliderType::AABB) {
                    


                 }
        }
    }
}


bool CollisionSystem::SphereAABBCollisionDetection(const Sphere& s, const AABB& aabb) const {
    //get center point circle first

    //glm::vec2 aabb_half_extents(two.Size.x / 2.0f, two.Size.y / 2.0f);
    Vec3 aabb_half_extents = Vec3(aabb.rx , aabb.ry , aabb.rz );
  
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

bool CollisionSystem::SphereSphereCollisionDetection(const Sphere& s1, const Sphere& s2) const {
    float distance = VMath::distance(s1.center, s2.center);
    if (distance < s1.r + s2.r) {
        return true;
    }
    return false;
}

bool CollisionSystem::SpherePlaneCollisionDetection(const Sphere& s1, const Plane& p1) const {
    /// Go Diana!
    float dist = VMath::dot(s1.center - p1.n, p1.n);
    //float distance = std::abs(VMath::dot(s1.center, p1.n) + p1.d);
    if (dist < s1.r) {
        return true;
    }
    else {
        return false; /// until you write it otherwise. 
    }
}



bool CollisionSystem::AABBAABBCollisionDetection(const AABB& bb1, const AABB& bb2) const {
    if (abs(bb1.center.x - bb2.center.x) > bb1.rx + bb2.rx) return false;
    if (abs(bb1.center.y - bb2.center.y) > bb1.ry + bb2.ry) return false;
    if (abs(bb1.center.z - bb2.center.z) > bb1.rz + bb2.rz) return false;
    return true;
}




void CollisionSystem::SphereSphereCollisionResponse(Sphere s1, Ref<PhysicsComponent> pc1, Sphere s2, Ref<PhysicsComponent> pc2) {
 
    
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

   if (!pc1->isStatic ) {
       pc1->vel = v1 + (v1p_new - v1p) * n;
   }
   if (!pc2->isStatic) {
       pc2->vel = v2 + (v2p_new - v2p) * n;
   }


   // pc1->vel = v1 + (v1p_new - v1p) * n;
    
}

void CollisionSystem::SphereAABBCollisionResponce(Sphere s, Ref<PhysicsComponent> pc1, AABB box, Ref<PhysicsComponent> pc2)
{
    // Find the closest point on the AABB to the sphere
    Vec3 aabb_half_extents = Vec3(box.rx, box.ry, box.rz);
    Vec3 difference = s.center - box.center;
    Vec3 clamped;
    clamped.x = std::max(-aabb_half_extents.x, std::min(difference.x, aabb_half_extents.x));
    clamped.y = std::max(-aabb_half_extents.y, std::min(difference.y, aabb_half_extents.y));
    clamped.z = std::max(-aabb_half_extents.z, std::min(difference.z, aabb_half_extents.z));
    Vec3 closest = box.center + clamped;

    // Calculate distance and normal
    Vec3 L = s.center - closest;
    float distance = VMath::mag(L);


    Vec3 n = VMath::normalize(L);
    float overlap = s.r - distance;

    //Static box and dynamic sphere (player character)
    if (pc2->isStatic && !pc1->isStatic) {
        // Position correction - move the sphere out of the box
        pc1->pos = pc1->pos + n * (overlap + 0.001f);

        // Project current velocity onto the collision plane instead of full bounce
        Vec3 v1 = pc1->vel;
        float dot_product = VMath::dot(v1, n);

        // If moving into the surface
        if (dot_product < 0) {
            // Remove the normal component from velocity (slide along the surface)
            pc1->vel = v1 - n * dot_product;

            // Optionally apply some friction along the surface
            float friction = 0.8f; // 1.0 = no friction, 0.0 = full stop
            pc1->vel = pc1->vel * friction;
        }
    }
    // For other cases (dynamic box, or both dynamic objects)
    else {
        // Original physics response - only if we need it
        if (!pc1->isStatic || !pc2->isStatic) {
            float e = 0.3f; // Lower coefficient of restitution for less bouncy collisions
            Vec3 v1 = pc1->vel;
            Vec3 v2 = pc2->vel;
            float m1 = pc1->mass;
            float m2 = pc2->mass;
            float v1p = VMath::dot(v1, n);
            float v2p = VMath::dot(v2, n);

            // Only apply physics response if objects are moving toward each other
            if (v1p - v2p <= 0.0f) {
                float v1p_new = ((m1 - e * m2) * v1p + ((1.0f + e) * m2 * v2p)) / (m1 + m2);
                float v2p_new = ((m2 - e * m1) * v2p + ((1.0f + e) * m1 * v1p)) / (m1 + m2);

                if (!pc1->isStatic) pc1->vel = v1 + (v1p_new - v1p) * n;
                if (!pc2->isStatic) pc2->vel = v2 + (v2p_new - v2p) * n;
            }

            // Position correction
            if (!pc1->isStatic && !pc2->isStatic) {
                // Share displacement between objects based on inverse mass ratio
                float totalMass = m1 + m2;
                float ratio1 = m2 / totalMass;
                float ratio2 = m1 / totalMass;
                s.center = s.center + n * (overlap * ratio1);
                box.center = box.center - n * (overlap * ratio2);
            }
            else if (!pc1->isStatic) {
                s.center = s.center + n * overlap;
            }
            else if (!pc2->isStatic) {
                box.center = box.center - n * overlap;
            }
        }
    }
}
