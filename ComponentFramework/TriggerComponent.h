#ifndef TRIGGER_COMPONENT_H
#define TRIGGER_COMPONENT_H
#include <Plane.h>
#include <functional>
using namespace MATHEX;
#include "Component.h"
#include "TransformComponent.h"


enum class TriggerType {
	Sphere
};

class Actor;
#pragma region Not_For_the_weak_do_not_open
/// <summary>
/// Trigger CallbackInterface to store the TriggerCallback*
/// ,this is because you can't just store templates without specifying a type.
/// This interface exists so we declare an empty reference without specifiying the type
/// </summary>
class TriggerCallback {
public:
	virtual void Call(std::shared_ptr<Actor> other) = 0;
};

/// <summary>
/// The actual object that stores the object reference and the function to call
/// </summary>
/// <typeparam name="T">Object type that is stored</typeparam>
template <class T>
class TriggerCallbackObject : public TriggerCallback {
public:
	TriggerCallbackObject(T* o_, void(T::* f_)(std::shared_ptr<Actor> other)) {
		obj = o_;
		func = f_;
	}

	void Call(std::shared_ptr<Actor> other) override {
		(obj->*func)(other);
	}
private:
	T* obj;
	void(T::* func)(std::shared_ptr<Actor> other);
};

/// <summary>
/// Static class that exists only for creating the TriggerCallback object
/// </summary>
class TriggerCallbackCreator {
public:
	template <typename T>
	static TriggerCallback* CreateTriggerCallback(T* obj, void(T::* function)(std::shared_ptr<Actor> other)){
		return new TriggerCallbackObject<T>(obj, function);
	}
};

#pragma endregion 


class TriggerComponent : public Component
{
	friend class TriggerSystem;
	TriggerComponent(const TriggerComponent&) = delete;
	TriggerComponent(TriggerComponent&&) = delete;
	TriggerComponent& operator = (const TriggerComponent&) = delete;
	TriggerComponent& operator = (TriggerComponent&&) = delete;
	TriggerCallback* callback = nullptr;

protected:
	TriggerType colliderType;
	float radius; /// sphere collision


public:
	TriggerComponent(Component* parent_, float radius_); /// Sphere 
	bool OnCreate() { return true; }
	void OnDestroy() {}
	void Update(const float deltaTime_) {}
	void Render()const {}
	void SetCallback(TriggerCallback* c) { callback = c; }
	void Call(std::shared_ptr<Actor> other) const{
		if (callback != nullptr) { 
			callback->Call(other); 
		}
	}
}; 

#endif // !TRIGGER_COMPONENT_H

