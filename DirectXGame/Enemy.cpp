#include "Enemy.h"
#include <cassert>
#include "MyMtMatrix.h"
#include "MyMtVector3.h"
#include "TextureManager.h"
#include "EnemyStateApproach.h"

void Enemy::Initialize(Model* model, const Vector3& position) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("./Resources/white1x1.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	//
	ChangeState(std::make_unique<EnemyStateApproach>(this));
}

void Enemy::Update() {
	// キャラ移動
	state_->Update();

	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection, textureHandle_); }

void Enemy::ChangeState(std::unique_ptr<BaseEnemyState> state) { state_ = std::move(state); }

void Enemy::SetWorldTransformTranslation(const Vector3& translation) { worldTransform_.translation_ = translation; }
