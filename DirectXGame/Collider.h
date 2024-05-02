#pragma once
#include "WorldTransform.h"

class Collider {
public:
	virtual ~Collider() {}

	virtual void OnCollision() = 0;

private:
	float radius_ = 1.0f;
	//
	uint32_t collisionAttribute_ = 0xffffffff;
	uint32_t collisionMask_ = 0xffffffff;

public:
	float GetRadius() { return radius_; }
	void SetRadius(const float& radius) { radius_ = radius; }
	virtual Vector3 GetWorldPosition() = 0;
	uint32_t GetCollisionAttribute() { return collisionAttribute_; }
	void SetCollisionAttribute(const uint32_t& collisionAttribute) { collisionAttribute_ = collisionAttribute; }
	uint32_t GetCollisionMask() { return collisionMask_; }
	void SetCollisionMask(const uint32_t& collisionMask) { collisionMask_ = collisionMask; }
};
