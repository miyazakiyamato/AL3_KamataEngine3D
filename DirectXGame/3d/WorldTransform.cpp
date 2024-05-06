#include "WorldTransform.h"
#include "MyMtMatrix.h"

void WorldTransform::UpdateMatrix() { 
	matWorld_ = MyMtMatrix::MakeAffineMatrix(scale_,rotation_,translation_);
	
	if (parent_) {
		matWorld_ = MyMtMatrix::Multiply( matWorld_,parent_->matWorld_);
	}

	TransferMatrix();
}