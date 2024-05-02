#pragma once
#include "Model.h"
#include "WorldTransform.h"

class EnemyBullet {
public:
	EnemyBullet() {}
	~EnemyBullet() {}
	
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	void Update(const Vector3& velocity);
	void Draw(const ViewProjection& viewProjection);
	//
	void OnCollision();

private:
	// 寿命
	static const int32_t kLifeTime = 60 * 5;

	Model* model_ = nullptr;
	//
	WorldTransform worldTransform_{};
	//
	uint32_t textureHandle_ = 0;
	//
	Vector3 velocity_{};
	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;
	//関数
	void Rotate(const Vector3& velocity);

public:
	bool isDead() const { return isDead_; }
	Vector3 GetWorldPosition();
};
