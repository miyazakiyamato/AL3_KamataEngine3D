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
}

void Enemy::Update() {
	// キャラ移動
	const float kCharacterSpeed = 0.2f;
	switch (phase_) {
	case Phase::Approach:
	default:
		ApproachUpdate({0.0f, 0.0f, -kCharacterSpeed});
		break;
	case Phase::Leave:
		LeaveUpdate({-kCharacterSpeed, kCharacterSpeed, 0.0f});
		break;
	}

	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection, textureHandle_); }

void Enemy::ApproachUpdate(const Vector3& move) {
	worldTransform_.translation_ = MyMtVector3::Add(worldTransform_.translation_, move);
	//
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::LeaveUpdate(const Vector3& move) { worldTransform_.translation_ = MyMtVector3::Add(worldTransform_.translation_, move); }
