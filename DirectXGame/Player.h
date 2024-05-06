#pragma once
#include "Model.h"
#include "Input.h"
#include "PlayerBullet.h"
#include <list>
#include "Collider.h"

class GameScene;

class Player:public Collider {
public:
	Player(){};

	~Player();
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

	void OnCollision() override;

private:
	GameScene* gameScene_ = nullptr;
	//ワールド変換データ
	WorldTransform worldTransform_{};
	//モデル
	Model* model_ = nullptr;
	//キー入力
	Input* input_ = nullptr;
	//
	float inputFloat3[3] = {0, 0, 0};
	//関数
	void Attack();
	void Rotate();
	
public:
	Vector3 GetWorldPosition() override;
	void SetParent(const WorldTransform* parent);
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }
};
