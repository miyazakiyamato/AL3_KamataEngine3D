#include "EnemyBullet.h"
#include "TextureManager.h"
#include <cassert>
#include "MyMtVector3.h"
#include "MyMtMatrix.h"
#include "CollisionConfig.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	Rotate(velocity);
	worldTransform_.UpdateMatrix();
	velocity_ = velocity;
	textureHandle_ = TextureManager::Load("./Resources/red.png");
	SetCollisionAttribute(kCollisionAttributeEnemy);
	//
	SetCollisionMask(GetCollisionMask() - kCollisionAttributeEnemy);
}

void EnemyBullet::Update(const Vector3& velocity) {
	Vector3 toPlayer = velocity;



	velocity_ = MyMtVector3::Multiply(MyMtVector3::Length(velocity), MyMtVector3::Slerp(MyMtVector3::Normalize( velocity_),MyMtVector3::Normalize( toPlayer), 0.1f));

	Rotate(velocity_);

	worldTransform_.translation_ = MyMtVector3::Add(worldTransform_.translation_, velocity_);
	worldTransform_.UpdateMatrix();
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection, textureHandle_); }

void EnemyBullet::OnCollision() { isDead_ = true; }

void EnemyBullet::Rotate(const Vector3& velocity) {
	worldTransform_.rotation_.y = std::atan2f(velocity.x, velocity.z);
	Vector3 velocityZ = MyMtMatrix::Transform(velocity, MyMtMatrix::MakeRotateYMatrix(-worldTransform_.rotation_.y));
	worldTransform_.rotation_.x = std::atan2f(-velocityZ.y, velocityZ.z);
}

Vector3 EnemyBullet::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

