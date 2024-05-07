#pragma once
#include "Model.h"
#include "Input.h"
#include "PlayerBullet.h"
#include <list>
#include "Collider.h"
#include "Sprite.h"

class GameScene;

class Player:public Collider {
public:
	Player(){};

	~Player();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureReticle, const Vector3& position);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update(const ViewProjection& viewProjection);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);
	void DrawUI();

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
	//2Dレティクル
	Sprite* sprite2DReticle_ = nullptr;
	//3Dレティクル
	WorldTransform worldTransform3DReticle_;

	bool IsPad_ = false;
	//関数
	void Attack();
	void Move();
	void Rotate();
	void Reticle2DUpdate(const ViewProjection& viewProjection);
	void Reticle3DUpdate();

public:
	Vector3 GetWorldPosition() override;
	Vector3 GetWorldPosition3DReticle();
	void SetParent(const WorldTransform* parent);
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }
};
