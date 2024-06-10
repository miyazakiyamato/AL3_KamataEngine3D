#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"

class RailCamera {
public:
	void Initialize(const Vector3& position,const Vector3& rotate);
	void Update();

private:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	
public:
	const WorldTransform& GetWorldTransform() { return worldTransform_; };
	const ViewProjection& GetViewProjection() { return viewProjection_; }
};
