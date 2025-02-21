#pragma once
#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

class Component {
	public:
		Component(Component* parent_):parent(parent_), isCreated(false)  {}

		virtual ~Component() = default; /// default just means {}
		virtual bool OnCreate() = 0;
		virtual void OnDestroy() = 0;
		virtual void Update(const float deltaTime_) = 0;
		virtual void Render()const = 0;
	protected:
		Component* parent;
		/// TODO: Add std::string name;
		bool isCreated;

};

