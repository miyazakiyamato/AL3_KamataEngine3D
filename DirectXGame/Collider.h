#pragma once
#include "WorldTransform.h"

class Collider {
public:
	virtual ~Collider() {}

	virtual void OnCollision() = 0;

private:
	float radius_ = 1.0f;

public:
	float GetRadius() { return radius_; }
	void SetRadius(const float& radius) { radius_ = radius; }
	virtual Vector3 GetWorldPosition() = 0;
};
