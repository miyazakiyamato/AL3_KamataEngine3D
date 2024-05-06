#include "RailCamera.h"
#include "MyMtVector3.h"
#include "MyMtMatrix.h"
#include "imgui.h"

void RailCamera::Initialize(const Vector3& position, const Vector3& rotate) {
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotate;
	worldTransform_.UpdateMatrix();
	//
	viewProjection_.farZ = 600.0f;
	viewProjection_.Initialize();
}

void RailCamera::Update() {
	Vector3 velocity = { 0.0f, 0.0f, 0.0f};
	Vector3 rotate = {0.0f, 0.00f, 0.0f};
	worldTransform_.translation_ = MyMtVector3::Add(worldTransform_.translation_, velocity);
	worldTransform_.rotation_ = MyMtVector3::Add(worldTransform_.rotation_, rotate);
	worldTransform_.UpdateMatrix();
	//
	viewProjection_.matView = MyMtMatrix::Inverse(worldTransform_.matWorld_);
	ImGui::Begin("camera");
	ImGui::DragFloat3("translation",&worldTransform_.translation_.x, 0.1f);
	ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.01f);
	ImGui::End();
}

