#pragma once
#include "Model.h"
#include "WorldTransform.h"

enum class Phase {
	Approach, // 接近する
	Leave,    // 離脱する
};

class Enemy {
public:
	Enemy(){};
	~Enemy(){};
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model,const Vector3& position);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	//フェーズ
	Phase phase_ = Phase::Approach;
	//関数
	void ApproachUpdate(const Vector3& move);
	void LeaveUpdate(const Vector3& move);
};
