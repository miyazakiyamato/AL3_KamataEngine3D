#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBullet.h"
#include "CollisionManager.h"
#include "Skydome.h"
#include "RailCamera.h"
#include <sstream>
#include <PrimitiveDrawer.h>

/// <summary>
/// ゲームシーン#
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void AddEnemy(const Vector3& position);
	void AddPlayerBullet(PlayerBullet* playerBullet);
	void AddEnemyBullet(EnemyBullet* enemyBullet);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	//テクスチャーハンドル
	uint32_t textureHandle_ = 0;
	uint32_t textureReticle_ = 0;
	//3Dモデル
	Model* model_ = nullptr;
	//
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	//
	Skydome* modelSkydome_ = nullptr;
	//
	Player* player_ = nullptr;
	std::list <Enemy*> enemy_;
	//
	std::list<PlayerBullet*> playerBullets_;
	std::list<EnemyBullet*> enemyBullets_;
	//
	bool isDebugCameraActive_ = false;
	//
	CollisionManager* collisionManager_ = nullptr;
	//
	RailCamera* railCamera_ = nullptr;
	//敵発生コマンド
	std::stringstream enemyPopCommands;
	int32_t enemyPopTimer_ = 0;
	bool IsEnemyPop_ = false;
	//
	std::vector<Vector3> controlPoints_;

	std::vector<Vector3> pointsDrawing;
	PrimitiveDrawer* primitiveDrawer = nullptr;
	//
	void SetAllCollisions();
	void BulletsUpdate();
	void LoadEnemyPopData();
	void UpdateEnemyPopCommands();
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
