#include "EnemyBullet.h"
#include "TextureManager.h"
#include <cassert>
#include "MyMtVector3.h"
#include "MyMtMatrix.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = {0.5f, 0.5f, 3.0f};
	worldTransform_.rotation_.y = std::atan2f(velocity.x,velocity.z);
	Vector3 velocityZ = MyMtMatrix::Transform(velocity, MyMtMatrix::MakeRotateYMatrix(-worldTransform_.rotation_.y));
	worldTransform_.rotation_.x = std::atan2f(-velocityZ.y, velocityZ.z);
	worldTransform_.UpdateMatrix();
	velocity_ = velocity;
	textureHandle_ = TextureManager::Load("./Resources/red.png");
}

void EnemyBullet::Update() {
	worldTransform_.translation_ = MyMtVector3::Add(worldTransform_.translation_, velocity_);
	worldTransform_.UpdateMatrix();
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection, textureHandle_); }

