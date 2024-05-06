#include "Player.h"
#include <cassert>
#include "MyMtMatrix.h"
#include "MyMtVector3.h"
#include "ImGuiManager.h"
#include "CollisionConfig.h"

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	//worldTransform_.scale_ = {2.0f, 2.0f, 2.0f};
	//worldTransform_.UpdateMatrix();
	//シングルトン
	input_ = Input::GetInstance();
	//
	SetCollisionAttribute(kCollisionAttributePlayer);
	//
	SetCollisionMask(GetCollisionMask() - kCollisionAttributePlayer);
}

void Player::Update() { 
	worldTransform_.TransferMatrix();
	//キャラ移動
	Vector3 move = {0, 0, 0};
	//
	const float kCharacterSpeed = 0.2f;
	//
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;	
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}
	Rotate();

	Attack();
	
#ifdef _DEBUG
	ImGui::Begin("Player");
	inputFloat3[0] = worldTransform_.translation_.x;
	inputFloat3[1] = worldTransform_.translation_.y;
	inputFloat3[2] = worldTransform_.translation_.z;
	ImGui::SliderFloat3("SliderFloat3", inputFloat3, -15.0f, 15.0f);
	worldTransform_.translation_ = {inputFloat3[0], inputFloat3[1], inputFloat3[2]};
	ImGui::End();
#endif
	//移動限界座標
	const float kMoveLimitX = 34.f;
	const float kMoveLimitY = 18.5f;
	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);
	//
	worldTransform_.translation_ = MyMtVector3::Add(worldTransform_.translation_, move);
	//
	//worldTransform_.matWorld_ = MyMtMatrix::MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	//
	worldTransform_.UpdateMatrix();
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::OnCollision() {}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {
		// DirectX::XMFLOAT3 position = worldTransform_.translation_;
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		velocity = MyMtMatrix::TransformNormal(velocity, worldTransform_.matWorld_);

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_,velocity);
		bullets_.push_back(newBullet);
	}
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->isDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
}

void Player::Rotate() { 
	const float kRotSpeed = 0.02f;
	//
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

