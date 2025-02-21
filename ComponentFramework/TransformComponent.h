#pragma once
#include "Component.h"
#include "Matrix.h"
#include "QMath.h"
#include "Euler.h"
using namespace MATH;
class TransformComponent : public Component {
	TransformComponent(const TransformComponent&) = delete;
	TransformComponent(TransformComponent&&) = delete;
	TransformComponent& operator = (const TransformComponent&) = delete;
	TransformComponent& operator = (TransformComponent&&) = delete;
protected:
	Vec3 pos;
	Vec3 scale;
	Quaternion orientation;

	/// Slerp stuff
	Quaternion start, end;
	float durationTime;
	float elapsedTime;
	bool doSlerpOrientation;
 
public:
	TransformComponent(Component* parent_);
	TransformComponent(Component* parent_,Vec3 pos_, Quaternion orientation_, Vec3 scale_ = Vec3(1.0f, 1.0f, 1.0f));
	~TransformComponent();
	bool OnCreate();
	void OnDestroy();
	void Update(const float deltaTime_);
	void Render() const;
	void SlerpOrientation(Quaternion start_, Quaternion end_, float durationTime_);


	/// Inlined methods 
	Vec3 GetPosition() { return pos; }
	Vec3 GetScale() { return scale; }
	Quaternion GetQuaternion() { return orientation; }
	void SetQuaternion(const Quaternion orientation_) { orientation = orientation_; }
	Matrix4 GetTransformMatrix() const;
	void SetTransform(const Vec3 pos_, const Quaternion orientation_, const Vec3 scale_ = Vec3(1.0f, 1.0f, 1.0f) ) {
		pos = pos_;
		orientation = orientation_;
		scale = scale_;
	}
	void SetPosition(const Vec3 pos_) { pos = pos_; }
	void SetScale(const Vec3 newScale_) { scale = newScale_; }

};

