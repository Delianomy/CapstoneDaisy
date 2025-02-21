#include "Actor.h"
#include "PhysicsSystem.h"


void PhysicsSystem::Update(const float deltaTime) {
    for (auto& actor : physicsActors) {
        Ref<PhysicsComponent> pc = actor->GetComponent<PhysicsComponent>();

        if (pc->useGravity == true) {
            Vec3 gravityForce(0.0f, gravity * pc->mass, 0.0f);
            pc->ApplyForce(gravityForce);
        }
        pc->accel = pc->force / pc->mass;
        pc->pos += pc->vel * deltaTime + 0.5f * pc->accel * deltaTime * deltaTime;
        pc->vel += pc->accel * deltaTime;
        pc->UndoForce();
    }

}
