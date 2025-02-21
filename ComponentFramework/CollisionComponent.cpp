#include "CollisionComponent.h"



CollisionComponent::CollisionComponent(Component* parent_, float radius_) :
	Component(parent_), radius(radius_) {
	colliderType = ColliderType::Sphere;

}
CollisionComponent::CollisionComponent(Component* parent_, Plane& plane_) :
	Component(parent_), plane(plane_) {
	colliderType = ColliderType::PLANE;

}

CollisionComponent::CollisionComponent(Component* parent_, AABB& aabb_) :
	Component(parent_), aabb(aabb_) {
	colliderType = ColliderType::AABB;
}