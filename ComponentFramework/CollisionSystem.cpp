#include "CollisionSystem.h"
#include <cmath>


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
           
            if ((collidingActors[i]->GetComponent<CollisionComponent>()->colliderType == ColliderType::Sphere &&
                collidingActors[j]->GetComponent<CollisionComponent>()->colliderType == ColliderType::PLANE)) {
                Sphere s1;
                s1.r = collidingActors[i]->GetComponent<CollisionComponent>()->radius;
                s1.center = collidingActors[i]->GetComponent<PhysicsComponent>()->pos;

                Plane p1;
                p1 = collidingActors[j]->GetComponent<CollisionComponent>()->plane;


                if (SpherePlaneCollisionDetection(s1, p1) == true) {
                    Ref<PhysicsComponent> pc1 = collidingActors[i]->GetComponent<PhysicsComponent>();
                    Ref<PhysicsComponent> pc2 = collidingActors[j]->GetComponent<PhysicsComponent>();
                    SpherePlaneCollisionResponse(s1, pc1, p1, pc2);
                   // std::cout << "Plane&Sphere" << std::endl;
                }
            
            }
            if (collidingActors[i]->GetComponent<CollisionComponent>()->colliderType == ColliderType::PLANE &&
                collidingActors[j]->GetComponent<CollisionComponent>()->colliderType == ColliderType::Sphere) {
                Sphere s1;
                s1.r = collidingActors[j]->GetComponent<CollisionComponent>()->radius;
                s1.center = collidingActors[j]->GetComponent<PhysicsComponent>()->pos;

                Plane p1;
                p1 = collidingActors[i]->GetComponent<CollisionComponent>()->plane;


                if (SpherePlaneCollisionDetection(s1, p1) == true) {
                    Ref<PhysicsComponent> pc1 = collidingActors[i]->GetComponent<PhysicsComponent>();
                    Ref<PhysicsComponent> pc2 = collidingActors[j]->GetComponent<PhysicsComponent>();
                    SpherePlaneCollisionResponse(s1, pc1, p1, pc2);
                    //std::cout << "Plane&Sphere" << std::endl;
                }
            
            }

        }
    }
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


void CollisionSystem::SpherePlaneCollisionResponse(const Sphere& s1, Ref<PhysicsComponent> pc1, const Plane& p1, Ref<PhysicsComponent> pc2) const {
    /*  float e = 1.0f; /// coefficient of restitution

      Vec3 L = s1.center - p1.n;
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
      }*/



      // Plane normal
    Vec3 planeNormal = Vec3(p1.x, p1.y, p1.z);

    // Debug print plane details
    std::cout << "Plane Normal: " << planeNormal.x << ", " << planeNormal.y << ", " << planeNormal.z << std::endl;
    std::cout << "Plane D: " << p1.d << std::endl;

    // Sphere details
    std::cout << "Sphere Center: " << s1.center.x << ", " << s1.center.y << ", " << s1.center.z << std::endl;
    std::cout << "Sphere Radius: " << s1.r << std::endl;

    // Calculate signed distance
    float signedDistance = VMath::dot(s1.center, planeNormal) + p1.d;

    std::cout << "Signed Distance: " << signedDistance << std::endl;
    std::cout << "Penetration Check: " << std::abs(signedDistance) << " < " << s1.r << std::endl;

    // Velocity details
    std::cout << "Current Velocity: "
        << pc1->vel.x << ", "
        << pc1->vel.y << ", "
        << pc1->vel.z << std::endl;

    // Explicit penetration resolution
    if (std::abs(signedDistance) < s1.r) {
        std::cout << "PENETRATION DETECTED!" << std::endl;

        // Determine correction direction
        Vec3 correctionDirection = (signedDistance < 0) ? planeNormal : -planeNormal;

        // Penetration depth
        float penetrationDepth = s1.r - std::abs(signedDistance);

        std::cout << "Correction Direction: "
            << correctionDirection.x << ", "
            << correctionDirection.y << ", "
            << correctionDirection.z << std::endl;
        std::cout << "Penetration Depth: " << penetrationDepth << std::endl;

        // Correction
        pc1->pos += correctionDirection * penetrationDepth;

        // Velocity reflection
        float normalVelocity = VMath::dot(pc1->vel, planeNormal);

        std::cout << "Normal Velocity: " << normalVelocity << std::endl;

        if (normalVelocity < 0) {
            float restitution = 0.8f;
            pc1->vel -= (1.0f + restitution) * normalVelocity * planeNormal;

            std::cout << "AFTER CORRECTION Velocity: "
                << pc1->vel.x << ", "
                << pc1->vel.y << ", "
                << pc1->vel.z << std::endl;
        }
    }
 

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
