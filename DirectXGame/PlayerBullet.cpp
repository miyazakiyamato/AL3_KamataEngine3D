#include "PlayerBullet.h"
#include "TextureManager.h"
#include <cassert>
#include "MyMtVector3.h"
#include "MyMtMatrix.h"
#include "CollisionConfig.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position,const Vector3 velocity) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("./Resources/black.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	velocity_ = velocity;
	Rotate(velocity_);
	SetCollisionAttribute(kCollisionAttributePlayer);
	//
	SetCollisionMask(GetCollisionMask() - kCollisionAttributePlayer);
}

void PlayerBullet::Update() {
	Rotate(velocity_);
	worldTransform_.translation_ = MyMtVector3::Add(worldTransform_.translation_, velocity_);
	worldTransform_.UpdateMatrix();
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw(ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection, textureHandle_); }

void PlayerBullet::OnCollision() { isDead_ = true; }

void PlayerBullet::Rotate(const Vector3& velocity) {
	worldTransform_.rotation_.y = std::atan2f(velocity.x, velocity.z);
	Vector3 velocityZ = MyMtMatrix::Transform(velocity, MyMtMatrix::MakeRotateYMatrix(-worldTransform_.rotation_.y));
	worldTransform_.rotation_.x = std::atan2f(-velocityZ.y, velocityZ.z);
}

Vector3 PlayerBullet::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}
