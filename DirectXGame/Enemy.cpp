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
	Vector3 move = {0, 0, 0};
	//
	const float kCharacterSpeed = 0.2f;

	move.z -= kCharacterSpeed;

	worldTransform_.translation_ = MyMtVector3::Add(worldTransform_.translation_, move);
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection, textureHandle_); }
