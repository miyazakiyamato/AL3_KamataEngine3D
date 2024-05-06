#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "MyMtMatrix.h"
#include "MyMtVector3.h"
#include "fstream"

GameScene::GameScene() {}

GameScene::~GameScene() {
	//
	delete model_;
	//
	delete debugCamera_;
	//
	delete player_;
	for (Enemy* enemy : enemy_) {
		delete enemy;
	}
	//
	for (PlayerBullet* bullet : playerBullets_) {
		delete bullet;
	}
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
	//
	delete collisionManager_;
	//
	delete modelSkydome_;
	//
	delete railCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	//
	textureHandle_ = TextureManager::Load("./Resources/cube/cube.jpg");
	model_ = Model::Create();
	worldTransform_.Initialize();
	viewProjection_.farZ = 600.0f;
	viewProjection_.Initialize();
	//
	debugCamera_ = new DebugCamera(1280, 720);
	//
	railCamera_ = new RailCamera();
	railCamera_->Initialize({0.0f, 0.0f, -50.0f}, {0.0f, 0.0f, 0.0f});
	//
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	//
	player_ = new Player(); 
	Vector3 playerPosition = {0.0f, 0.0f, 50.0f};
	player_->Initialize(Model::CreateFromOBJ("airship", true), playerPosition);
	player_->SetParent(&railCamera_->GetWorldTransform());
	player_->SetGameScene(this);

	LoadEnemyPopData();
	//
	collisionManager_ = new CollisionManager();

	modelSkydome_ = new Skydome();
	modelSkydome_->Initialize(Model::CreateFromOBJ("skydome", true));
	
	}

void GameScene::Update() { 
	debugCamera_->Update();
	//
	player_->Update();

	UpdateEnemyPopCommands();
	for (Enemy* enemy : enemy_) {
		enemy->Update();
	}
	enemy_.remove_if([](Enemy* enemy) {
		if (enemy->isDead()) {
			delete enemy;
			return true;
		}
		return false;
	});
	//
	BulletsUpdate();
	//
	modelSkydome_->Update();
	//
	collisionManager_->ColliderClear();
	SetAllCollisions();
	collisionManager_->CheckAllCollisions();
	//
	railCamera_->Update();
	viewProjection_.matView = railCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
	viewProjection_.TransferMatrix();
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
		//viewProjection_.UpdateMatrix();
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
	
	for (Enemy* enemy : enemy_) {
		enemy->Draw(viewProjection_);
	}
	//
	for (PlayerBullet* bullet : playerBullets_) {
		bullet->Draw(viewProjection_);
	}
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
	}
	//
	modelSkydome_->Draw(viewProjection_);
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

void GameScene::SetAllCollisions() {
	//
	collisionManager_->SetColliders(player_);
	for (Enemy* enemy : enemy_) {
		collisionManager_->SetColliders(enemy);
	}
	//
	for (PlayerBullet* bullet : playerBullets_) {
		collisionManager_->SetColliders(bullet);
	}
	for (EnemyBullet* bullet : enemyBullets_) {
		collisionManager_->SetColliders(bullet);
	}
}

void GameScene::BulletsUpdate() {
	for (PlayerBullet* bullet : playerBullets_) {
		bullet->Update();
	}
	playerBullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->isDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	//
	for (EnemyBullet* bullet : enemyBullets_) {
		const float kBulletSpeed = 1.0f;

		Vector3 velocity{MyMtVector3::Subtract(player_->GetWorldPosition(), bullet->GetWorldPosition())};
		velocity = MyMtVector3::Multiply(kBulletSpeed, MyMtVector3::Normalize(velocity));

		bullet->Update(velocity);
	}
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->isDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
}

void GameScene::LoadEnemyPopData() {
	std::ifstream file;
	file.open("./enemyPop.csv");
	assert(file.is_open());
	//
	enemyPopCommands << file.rdbuf();
	//
	file.close();
}

void GameScene::UpdateEnemyPopCommands() {
	//待機処理
	if (IsEnemyPop_) {
		enemyPopTimer_--;
		if (enemyPopTimer_ <= 0) {
			//
			IsEnemyPop_ = false;
		}
		return;
	}
	//
	std::string line;
	while (getline(enemyPopCommands, line)) {
		std::stringstream line_stream(line);

		std::string word;
		//
		getline(line_stream, word, ',');
		if (word.find("//") == 0) {
			continue;
		}
		//POP
		if (word.find("POP") == 0) {
			//x
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
			//y
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());
			//z
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());
			//
			AddEnemy(Vector3(x, y, z));
		}
		//WAIT
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');
			//
			int32_t waitTime = atoi(word.c_str());
			//待機開始
			IsEnemyPop_ = true;
			enemyPopTimer_ = waitTime;
			//
			break;
		}
	}

}

void GameScene::AddEnemy(const Vector3& position) {
	Enemy* newEnemy = new Enemy();
	newEnemy->SetPlayer(player_);
	newEnemy->SetGameScene(this);
	newEnemy->Initialize(model_,position);
	enemy_.push_back(newEnemy);
}

void GameScene::AddPlayerBullet(PlayerBullet* playerBullet) { playerBullets_.push_back(playerBullet); }

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) { enemyBullets_.push_back(enemyBullet); }
