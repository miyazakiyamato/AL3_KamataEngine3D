#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "BaseEnemyState.h"


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

	void ChangeState(std::unique_ptr<BaseEnemyState> state);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	//
	std::unique_ptr<BaseEnemyState> state_;

public:
	Vector3 GetWorldTransformTranslation() const{ return worldTransform_.translation_; }
	void SetWorldTransformTranslation(const Vector3& translation);
};
