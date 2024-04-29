#include "EnemyStateLeave.h"
#include "MyMtVector3.h"

EnemyStateLeave::EnemyStateLeave(Enemy* enemy) : BaseEnemyState("State Leave", enemy) {

}

void EnemyStateLeave::Update() {
	const float kCharacterSpeed = 0.2f;
	Vector3 move = {-kCharacterSpeed, kCharacterSpeed, 0.0f};
	enemy_->SetWorldTransformTranslation(MyMtVector3::Add(enemy_->GetWorldTransformTranslation(), move));
}
