#pragma once
#include "Model.h"
#include "WorldTransform.h"

class PlayerBullet {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model,const Vector3& position ,const Vector3 velocity);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

private:
	// 寿命
	static const int32_t kLifeTime = 60 * 5;

	Vector3 velocity_{};
	// ワールド変換データ
	WorldTransform worldTransform_{};
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;

public:
	bool isDead() const { return isDead_; }
};
