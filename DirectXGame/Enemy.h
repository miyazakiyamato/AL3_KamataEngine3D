#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "BaseEnemyState.h"
#include "EnemyBullet.h"
#include "list"
#include "TimedCall.h"

class Player;

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
	void FireTimed();
	void FireCancel();

private:
	Player* player_ = nullptr;
	// ワールド変換データ
	WorldTransform worldTransform_{};
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	//ステート
	std::unique_ptr<BaseEnemyState> state_;
	//リスト
	std::list<EnemyBullet*> bullets_;
	//時限発射のリスト
	std::list<TimedCall*> timedCalls_;
	//関数
	void Attack();

public:
	Vector3 GetWorldTransformTranslation() const{ return worldTransform_.translation_; }
	void SetWorldTransformTranslation(const Vector3& translation);
	void SetPlayer(Player* player) { player_ = player; }
	Vector3 GetWorldPosition();
};
