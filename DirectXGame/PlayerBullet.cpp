#include "PlayerBullet.h"
#include "TextureManager.h"
#include <cassert>
#include "MyMtVector3.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position,const Vector3 velocity) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("./Resources/black.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	velocity_ = velocity;
}

void PlayerBullet::Update() {
	worldTransform_.translation_ = MyMtVector3::Add(worldTransform_.translation_, velocity_);
	worldTransform_.UpdateMatrix();
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw(ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection, textureHandle_); }

void PlayerBullet::OnCollision() { isDead_ = true; }

Vector3 PlayerBullet::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}
