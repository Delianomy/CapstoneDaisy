#pragma once
#include "TransformComponent.h"
class PhysicsComponent : public TransformComponent {
	friend class PhysicsSystem;
	friend class CollisionSystem;
	friend class TriggerSystem;
	friend class AISystem;

	PhysicsComponent(const PhysicsComponent&) = delete;
	PhysicsComponent(PhysicsComponent&&) = delete;
	PhysicsComponent& operator = (const PhysicsComponent&) = delete;
	PhysicsComponent& operator = (PhysicsComponent&&) = delete;
protected:
	Vec3 vel;
	Vec3 accel;
	Vec3 force;
	

public:
	bool isStatic = false;
	bool useGravity = false;
	float mass =1.0f;
	PhysicsComponent(Component* parent_, Vec3 pos_, Quaternion orientation_,
		Vec3 vel_ = Vec3(0.0f, 0.0f, 0.0f), Vec3 accel_ = Vec3(0.0f, 0.0f, 0.0f),
		Vec3 force_ = Vec3(0.0f, 0.0f, 0.0f), Vec3 scale_ = Vec3(1.0f, 1.0f, 1.0f));
	bool OnCreate();
	void ApplyForce(const Vec3 force_);
	void UndoForce();
	void SetScale(const Vec3 scale_) { scale = scale_; }
	void SetVel(const Vec3 vel_) { vel = vel_; }
};


