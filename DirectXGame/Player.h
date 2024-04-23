#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"

class Player {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	//キー入力
	Input* input_ = nullptr;
	//
	float inputFloat3[3] = {0, 0, 0};

	//関数
	void Rotate();
};
