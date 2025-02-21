#include <iostream>
#include "TransformComponent.h"
#include "QMath.h"
#include "MMath.h"
using namespace MATH;

TransformComponent::TransformComponent(Component* parent_):Component(parent_) {
	pos = Vec3(0.0f, 0.0f, 0.0f);
	orientation = Quaternion(1.0f, Vec3(0.0f, 0.0f, 0.0f));
	scale = Vec3(1.0f, 1.0f, 1.0f);
	doSlerpOrientation = false;
	elapsedTime = 0.0f;
}

TransformComponent::TransformComponent(Component* parent_, Vec3 pos_, Quaternion orientation_, Vec3 scale_):
	Component{ parent_ }, pos{ pos_ }, orientation{ orientation_ }, scale{ scale_ },
	doSlerpOrientation{ false }, elapsedTime{ 0.0f }, durationTime{ 0.0f } {
}

bool TransformComponent::OnCreate() {
	if (isCreated) return isCreated;
	isCreated = true;
	return isCreated;
}

void TransformComponent::SlerpOrientation(Quaternion start_, Quaternion end_, float durationTime_) {
	doSlerpOrientation = true;
	start = start_;
	end = end_;
	durationTime = durationTime_;
	elapsedTime = 0.0f;
}


void TransformComponent::Update(const float deltaTime) {
	if (doSlerpOrientation == false) return;
	elapsedTime += deltaTime;
	float t = elapsedTime / durationTime;
	
	if (t > 1.0f) {
		t = 1.0f;
 		doSlerpOrientation = false;
		elapsedTime = 0.0f;
	}
	orientation = QMath::slerp(start, end, t);
}

void TransformComponent::Render() const {

}

Matrix4 TransformComponent::GetTransformMatrix() const {
	return MMath::translate(pos) * MMath::scale(scale) * MMath::toMatrix4(orientation);
}

TransformComponent::~TransformComponent() {
	OnDestroy();
	isCreated = false;
}

void TransformComponent::OnDestroy() {
	isCreated = false;
}