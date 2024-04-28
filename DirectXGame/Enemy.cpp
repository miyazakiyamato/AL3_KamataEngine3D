#include "Enemy.h"
#include <cassert>
#include "MyMtMatrix.h"
#include "MyMtVector3.h"
#include "TextureManager.h"

void Enemy::Initialize(Model* model, const Vector3& position) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("./Resources/white1x1.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	//
}

void Enemy::Update() {
	// キャラ移動
	(this->*spFuncTable[static_cast<size_t>(phase_)])();

	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection, textureHandle_); }

void Enemy::ApproachUpdate() {
	const float kCharacterSpeed = 0.2f;
	Vector3 move = {0.0f, 0.0f, -kCharacterSpeed};
	worldTransform_.translation_ = MyMtVector3::Add(worldTransform_.translation_, move);
	//
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::LeaveUpdate() {
	const float kCharacterSpeed = 0.2f;
	Vector3 move = {-kCharacterSpeed, kCharacterSpeed, 0.0f};
	worldTransform_.translation_ = MyMtVector3::Add(worldTransform_.translation_, move); }

void (Enemy::*Enemy::spFuncTable[])() = {
    &Enemy::ApproachUpdate,
    &Enemy::LeaveUpdate,
};