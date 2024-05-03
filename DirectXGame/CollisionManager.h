#pragma once
#include "list"
#include "Collider.h"
class CollisionManager {
public:
	CollisionManager() {}
	~CollisionManager();
	void CheckAllCollisions();

private:
	//コライダーリスト
    std::list<Collider*> colliders_;
	//
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

public:
	void SetColliders(Collider* colliders) { colliders_.push_back(colliders); } 
	void ColliderClear();
};
