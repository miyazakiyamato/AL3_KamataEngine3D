#pragma once
#include "Model.h"
#include "Input.h"
#include "PlayerBullet.h"
#include <list>
#include "Collider.h"

class Player:public Collider {
public:
	Player(){};

	~Player();
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

	void OnCollision() override;

private:
	//ワールド変換データ
	WorldTransform worldTransform_{};
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	//キー入力
	Input* input_ = nullptr;
	//
	float inputFloat3[3] = {0, 0, 0};
	//
	std::list<PlayerBullet*> bullets_;
	//関数
	void Attack();
	void Rotate();
	
public:
	Vector3 GetWorldPosition() override;
	const std::list<PlayerBullet*> GetBullets() { return bullets_; }
};
