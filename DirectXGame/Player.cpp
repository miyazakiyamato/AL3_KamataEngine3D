#include "Player.h"
#include <cassert>
#include "MyMtMatrix.h"
#include "MyMtVector3.h"
#include "ImGuiManager.h"
#include "CollisionConfig.h"
#include "GameScene.h"
#include "TextureManager.h"

Player::~Player() { delete sprite2DReticle_; }

void Player::Initialize(Model* model, uint32_t textureReticle, const Vector3& position) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	 worldTransform_.translation_ = position;
	 worldTransform_.UpdateMatrix();
	 //2Dスプライト
	 sprite2DReticle_ = Sprite::Create(textureReticle, {}, {1, 1, 1, 1}, {0.5f, 0.5f});	 
	 //3Dレティクル
	 worldTransform3DReticle_.Initialize();
	// シングルトン
	input_ = Input::GetInstance();
	//
	SetCollisionAttribute(kCollisionAttributePlayer);
	//
	SetCollisionMask(GetCollisionMask() - kCollisionAttributePlayer);
}

void Player::Update(const ViewProjection& viewProjection) { 
	worldTransform_.TransferMatrix();
	//キャラ移動
	Move();

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
	//worldTransform_.matWorld_ = MyMtMatrix::MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	//
	worldTransform_.UpdateMatrix();

	Reticle3DUpdate();
	Reticle2DUpdate(viewProjection);
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
	//model_->Draw(worldTransform3DReticle_, viewProjection);
}

void Player::DrawUI() { sprite2DReticle_->Draw(); }

void Player::OnCollision() {}

void Player::Attack() {
	XINPUT_STATE joyState;
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		IsPad_ = false;
	}
	if (input_->PushKey(DIK_SPACE) && !IsPad_ || joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER && IsPad_) {
		// DirectX::XMFLOAT3 position = worldTransform_.translation_;
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		velocity = MyMtVector3::Subtract(GetWorldPosition3DReticle(), GetWorldPosition());
		velocity = MyMtVector3::Normalize(MyMtVector3::Multiply(kBulletSpeed, velocity));

		velocity = MyMtMatrix::TransformNormal(velocity, worldTransform_.matWorld_);

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);
		gameScene_->AddPlayerBullet(newBullet);
	}
}

void Player::Move() {
	Vector3 move{};
	const float kCharacterSpeed = 0.4f;
	if (input_->TriggerKey(DIK_T)) {
		IsPad_ = !IsPad_;
	}
	if (IsPad_) {
		XINPUT_STATE joyState;
		if (input_->GetInstance()->GetJoystickState(0, joyState)) {
			move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
			move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
		}
	} else {
		//キーボード
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
	}
	worldTransform_.translation_ = MyMtVector3::Add(worldTransform_.translation_, move);
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

void Player::Reticle2DUpdate(const ViewProjection& viewProjection) {
	if (IsPad_) {
		Vector2 spritePosition = sprite2DReticle_->GetPosition();

		XINPUT_STATE joyState;
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			spritePosition.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
			spritePosition.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 5.0f;
			//
			sprite2DReticle_->SetPosition(spritePosition);
		}
	} else {
		POINT mousePosition;
		GetCursorPos(&mousePosition);
		//
		HWND hwnd = WinApp::GetInstance()->GetHwnd();
		ScreenToClient(hwnd, &mousePosition);
		//
		sprite2DReticle_->SetPosition(Vector2((float)mousePosition.x, (float)mousePosition.y));
	}
	
	//
	Matrix4x4 matViewport = MyMtMatrix::MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	Matrix4x4 matVPV = MyMtMatrix::Multiply(viewProjection.matView, MyMtMatrix::Multiply(viewProjection.matProjection, matViewport));
	//
	Matrix4x4 matInverseVPV = MyMtMatrix::Inverse(matVPV);
	//
	Vector3 posNear = Vector3(sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y, 0);
	Vector3 posFar = Vector3(sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y, 1);
	//
	posNear = MyMtMatrix::Transform(posNear, matInverseVPV);
	posFar = MyMtMatrix::Transform(posFar, matInverseVPV);
	//
	Vector3 mouseDirection = MyMtVector3::Subtract(posFar, posNear);
	mouseDirection = MyMtVector3::Normalize(mouseDirection);
	//
	const float kDistanceTestObject = 100.f;
	worldTransform3DReticle_.translation_ = MyMtVector3::Add(posNear, MyMtVector3::Multiply(kDistanceTestObject, mouseDirection));
	worldTransform3DReticle_.UpdateMatrix();
	//
	//
	Vector3 positionReticle = GetWorldPosition3DReticle();
	//
	positionReticle = MyMtMatrix::Transform(positionReticle, matVPV);
	//
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
}

void Player::Reticle3DUpdate() {
	//自機から３Dベクトルへの距離
	const float kDistancePlayerTo3DReticle = 50.f;
	//自機から３Dベクトルのオフセット
	Vector3 offset = {0, 0, 1.0f};
	//
	offset = MyMtMatrix::Transform(offset, worldTransform_.matWorld_);
	offset = MyMtVector3::Normalize(MyMtVector3::Multiply(kDistancePlayerTo3DReticle, offset));
	//
	worldTransform3DReticle_.translation_ = MyMtVector3::Add(worldTransform_.translation_, offset);
	worldTransform3DReticle_.UpdateMatrix();
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

Vector3 Player::GetWorldPosition3DReticle() {
	Vector3 worldPos;
	worldPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
	worldPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
	worldPos.z = worldTransform3DReticle_.matWorld_.m[3][2];
	return worldPos;
}

void Player::SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

