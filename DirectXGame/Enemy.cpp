#include "Enemy.h"
#include <cassert>
#include "MyMtMatrix.h"
#include "MyMtVector3.h"
#include "TextureManager.h"
#include "EnemyStateApproach.h"
#include "Player.h"

Enemy::~Enemy() {
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
	for (TimedCall* timedCalls : timedCalls_) {
		delete timedCalls;
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
}

void Enemy::Update() {
	// キャラ移動
	state_->Update();
	//
	Attack();
	//
	timedCalls_.remove_if([](TimedCall* timedCalls) {
		if (timedCalls->IsFinished()) {
			delete timedCalls;
			return true;
		}
		return false;
	});
	for (TimedCall* timedCalls : timedCalls_) {
		timedCalls->Update();
	}
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

void Enemy::FireTimed() {
	Fire();
	//
	timedCalls_.push_back(new TimedCall(std::bind_front(&Enemy::FireTimed, this), kFireInterval));
}

void Enemy::FireCancel() {
	timedCalls_.remove_if([](TimedCall* timedCalls) {
		delete timedCalls;
		return true;
	});
}

void Enemy::OnCollision() {}

void Enemy::Fire() {
	assert(player_);
	//
	const float kBulletSpeed = 1.0f;

	Vector3 velocity{MyMtVector3::Subtract(player_->GetWorldPosition(), GetWorldPosition())};
	velocity = MyMtVector3::Multiply(kBulletSpeed,MyMtVector3::Normalize(velocity));

	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);
	bullets_.push_back(newBullet);
}

void Enemy::Attack() {
	for (EnemyBullet* bullet : bullets_) {
		const float kBulletSpeed = 1.0f;

		Vector3 velocity{MyMtVector3::Subtract(player_->GetWorldPosition(), bullet->GetWorldPosition())};
		velocity = MyMtVector3::Multiply(kBulletSpeed, MyMtVector3::Normalize(velocity));

		bullet->Update(velocity);
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

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}
