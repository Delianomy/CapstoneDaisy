#ifndef TRIGGER_COMPONENT_H
#define TRIGGER_COMPONENT_H
#include <Plane.h>
using namespace MATHEX;


#include "Component.h"
#include "TransformComponent.h"

enum class TriggerType {
	Sphere
};


class TriggerComponent : public Component
{
	friend class TriggerSystem;
	TriggerComponent(const TriggerComponent&) = delete;
	TriggerComponent(TriggerComponent&&) = delete;
	TriggerComponent& operator = (const TriggerComponent&) = delete;
	TriggerComponent& operator = (TriggerComponent&&) = delete;
protected:
	
	TriggerType colliderType;
	float radius; /// sphere collision


public:
	TriggerComponent(Component* parent_, float radius_); /// Sphere 
	bool OnCreate() { return true; }
	void OnDestroy() {}
	void Update(const float deltaTime_) {}
	void Render()const {}
};

#endif // !TRIGGER_COMPONENT_H

