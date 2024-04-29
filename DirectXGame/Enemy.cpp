#include "Enemy.h"
#include <cassert>
#include "MyMtMatrix.h"
#include "MyMtVector3.h"
#include "TextureManager.h"
#include "EnemyStateApproach.h"

Enemy::~Enemy() {
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Enemy::Initialize(Model* model, const Vector3& position) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("./Resources/white1x1.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	//
	ChangeState(std::make_unique<EnemyStateApproach>(this));
	//
	FireInitialize();
}

void Enemy::Update() {
	// キャラ移動
	state_->Update();
	//
	Attack();
	//
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::ChangeState(std::unique_ptr<BaseEnemyState> state) { state_ = std::move(state); }

void Enemy::FireInitialize() { fireTimer_ = kFireInterval; }

void Enemy::Fire() {
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, -kBulletSpeed);

	velocity = MyMtMatrix::TransformNormal(velocity, worldTransform_.matWorld_);

	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);
	bullets_.push_back(newBullet);
}

void Enemy::Attack() {
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->isDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
}

void Enemy::SetWorldTransformTranslation(const Vector3& translation) { worldTransform_.translation_ = translation; }

void Enemy::SetFireTimer(const int32_t& fireTimer) { fireTimer_ = fireTimer; }
