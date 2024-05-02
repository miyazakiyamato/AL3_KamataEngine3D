#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "MyMtMatrix.h"
#include "MyMtVector3.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	//
	delete model_;
	//
	delete debugCamera_;
	//
	delete player_;
	delete enemy_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	//
	textureHandle_ = TextureManager::Load("./Resources/cube/cube.jpg");
	model_ = Model::Create();
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	//
	debugCamera_ = new DebugCamera(1280, 720);
	//
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	//
	player_ = new Player(); 
	player_->Initialize(model_, textureHandle_);

	enemy_ = new Enemy();
	enemy_->SetPlayer(player_);
	enemy_->Initialize(model_, {30,2.0f,40.0f});
}

void GameScene::Update() { 
	debugCamera_->Update();
	//
	player_->Update();
	enemy_->Update();
	//
	CheckAllCollisions();
	// デバッグ表示
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_P)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		//ビュープロジェクション
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	player_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);
	//
	
	//デバッグ表示
#ifdef _DEBUG
	

#endif
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {
	Vector3 posA, posB;

	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	//
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();
	//
	posA = player_->GetWorldPosition();

	for (EnemyBullet* bullet : enemyBullets) {
		posB = bullet->GetWorldPosition();

		float length = powf(posB.x - posA.x, 2) + powf(posB.y - posA.y, 2) + powf(posB.z - posA.z, 2);
		if (length <= powf(1.0f + 1.0f, 2)) {
			player_->OnCollision();
			bullet->OnCollision();
		}
	}
	//
	posA = enemy_->GetWorldPosition();

	for (PlayerBullet* bullet : playerBullets) {
		posB = bullet->GetWorldPosition();

		float length = powf(posB.x - posA.x, 2) + powf(posB.y - posA.y, 2) + powf(posB.z - posA.z, 2);
		if (length <= powf(1.0f + 1.0f, 2)) {
			enemy_->OnCollision();
			bullet->OnCollision();
		}
	}
	//
	for (PlayerBullet* bullet1 : playerBullets) {
		posA = bullet1->GetWorldPosition();
		for (EnemyBullet* bullet2 : enemyBullets) {
			posB = bullet2->GetWorldPosition();

			float length = powf(posB.x - posA.x, 2) + powf(posB.y - posA.y, 2) + powf(posB.z - posA.z, 2);
			if (length <= powf(1.0f + 1.0f, 2)) {
				bullet1->OnCollision();
				bullet2->OnCollision();
			}
		}
	}
}
