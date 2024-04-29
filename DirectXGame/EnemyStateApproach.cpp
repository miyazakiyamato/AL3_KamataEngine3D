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
	if (enemy_->GetWorldTransformTranslation().z < 0.0f) {
		enemy_->ChangeState(std::make_unique<EnemyStateLeave>(enemy_));
	}
}
