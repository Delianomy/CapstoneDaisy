#include "TriggerComponent.h"
#include "Actor.h"

TriggerComponent::TriggerComponent(Component* parent_, float radius_):Component(parent_), radius(radius_) 	
{
		colliderType = TriggerType::Sphere;
}
