#pragma once
#include <Plane.h>
using namespace MATHEX;

#include "TransformComponent.h"

enum class ColliderType {
	Sphere,
	AABB,
	PLANE,
	RECTANGLE,
	OBB
};

/// Definition of a Sphere and Plane are in the Math library

/// For an Axis Aligned Bounding Box, their are many ways you could use to define the box.
/// My favorite way, the easiest to understand way, is this way.
/// Pick the center location of the box, then specify the radius from that center in 
/// the x, y,and z dimensions.
/// Umer does it the same way, he calls the radius the extent

struct AABB {
	Vec3 center;
	float rx, ry, rz;
};

/*** Other definitions might be:
struct AABB {
	Vec3 min; /// Bottom, left, front
	Vec3 max; /// Top, right, back
};

struct AABB {
	Vec3 min; /// Bottom, left, front
	float dx,dy,dz;
};
***/

class CollisionComponent : public Component {
	friend class CollisionSystem;
	CollisionComponent(const CollisionComponent&) = delete;
	CollisionComponent(CollisionComponent&&) = delete;
	CollisionComponent& operator = (const CollisionComponent&) = delete;
	CollisionComponent& operator = (CollisionComponent&&) = delete;
protected:
	ColliderType colliderType;
	float radius; /// sphere collision
	Plane plane;
	AABB aabb; /// AABB data
	Quaternion orientation;


public:
	
	CollisionComponent(Component* parent_, float radius_); /// Sphere 
	CollisionComponent(Component* parent_, Plane& plane); /// Plane
	CollisionComponent(Component* parent_, AABB& aabb);   /// AABB
	bool OnCreate() { return true; }
	void OnDestroy() {}
	void Update(const float deltaTime_) {}
	void Render()const {}
	
};


