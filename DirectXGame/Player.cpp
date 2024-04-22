#include "Player.h"
#include <cassert>
#include "MyMtMatrix.h"
#include "MyMtVector3.h"
#include "ImGuiManager.h"

void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	//シングルトン
	input_ = Input::GetInstance();
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
#ifdef _DEBUG
	ImGui::Begin("Player");
	ImGui::SliderFloat3("SliderFloat3", inputFloat3,-15.0f,15.0f);
	ImGui::End();
#endif
	//worldTransform_.translation_ = {inputFloat3[0], inputFloat3[1], inputFloat3[2]};
	worldTransform_.matWorld_ = MyMtMatrix::MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

