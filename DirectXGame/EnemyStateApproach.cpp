#include "EnemyStateApproach.h"
#include "MyMtVector3.h"
#include "EnemyStateLeave.h"

EnemyStateApproach::EnemyStateApproach(Enemy* enemy)
	: BaseEnemyState("State Approach",enemy) {
}

void EnemyStateApproach::Update() {
	const float kCharacterSpeed = 0.2f;
	Vector3 move = {0.0f, 0.0f, -kCharacterSpeed};
	enemy_->SetWorldTransformTranslation(MyMtVector3::Add(enemy_->GetWorldTransformTranslation(), move));
	//発射タイマー
	enemy_->SetFireTimer(enemy_->GetFireTimer() - 1);
	if (enemy_->GetFireTimer() <= 0) {
		enemy_->Fire();
		//
		enemy_->SetFireTimer(enemy_->kFireInterval);
	}
	//
	if (enemy_->GetWorldTransformTranslation().z < 0.0f) {
		enemy_->ChangeState(std::make_unique<EnemyStateLeave>(enemy_));
	}
}
