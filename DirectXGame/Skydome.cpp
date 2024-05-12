#include "Skydome.h"
#include <cassert>

Skydome::~Skydome() { delete model_; }

void Skydome::Initialize(Model* model) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale_ = {3.0f, 3.0f, 3.0f};
	worldTransform_.UpdateMatrix();
}

void Skydome::Update() {}

void Skydome::Draw(ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection); }

