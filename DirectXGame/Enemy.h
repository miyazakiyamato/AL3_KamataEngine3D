#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "BaseEnemyState.h"
#include "EnemyBullet.h"
#include "list"

class Enemy {
public:
	Enemy(){};
	~Enemy();
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
	//発射間隔
	static const int kFireInterval = 60;
	//
	void Fire();
	void FireInitialize();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	//ステート
	std::unique_ptr<BaseEnemyState> state_;
	//リスト
	std::list<EnemyBullet*> bullets_;
	//発射タイマー
	int32_t fireTimer_ = 0;
	//関数
	void Attack();

public:
	Vector3 GetWorldTransformTranslation() const{ return worldTransform_.translation_; }
	void SetWorldTransformTranslation(const Vector3& translation);
	int32_t GetFireTimer() { return fireTimer_; }
	void SetFireTimer(const int32_t& fireTimer);
};
