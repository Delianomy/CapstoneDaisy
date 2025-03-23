#include "PhysicsComponent.h"


PhysicsComponent::PhysicsComponent(Component* parent_, Vec3 pos_, Quaternion orientation_,
	Vec3 vel_, Vec3 accel_,
	Vec3 force_, Vec3 scale_) :
	TransformComponent(parent_) {

	/// Set these values in a amateurish style but it is easier to read
	pos = pos_;
	vel = vel_;
	accel = accel_;
	force = force_;
	orientation = orientation_;
	scale = scale_;
	mass = 1.0f;
	useGravity = false;
}

PhysicsComponent::PhysicsComponent(Component* parent_, Vec3 pos_, bool isStatic_, bool useGravity_) :
	TransformComponent(parent_) {
	pos = pos_;
	vel = Vec3();
	accel = Vec3();
	force = Vec3();
	orientation = Quaternion();
	scale = Vec3(1.0, 1.0, 1.0);
	mass = 1.0f;
	isStatic = isStatic_;
	useGravity = useGravity;
};

bool PhysicsComponent::OnCreate() {
	if (isCreated) return isCreated;
	isCreated = true;
	return isCreated;
}
void PhysicsComponent::ApplyForce(const Vec3 force_) {
	force += force_;
}

void PhysicsComponent::UndoForce() {
	force = Vec3(0.0f, 0.0f, 0.0f);
}

