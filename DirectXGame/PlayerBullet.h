#pragma once
#include "Model.h"
#include "Collider.h"

class PlayerBullet : public Collider{
public:
	~PlayerBullet() {}
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
	//
	void OnCollision() override;

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
	//
	void Rotate(const Vector3& velocity);

public:
	bool isDead() const { return isDead_; }
	Vector3 GetWorldPosition() override;
};
