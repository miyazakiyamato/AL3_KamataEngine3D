#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerBullet.h"
#include <list>

class Player {
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
	Vector3 GetWorldPosition();

};
