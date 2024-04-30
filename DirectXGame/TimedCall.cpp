#include "TimedCall.h"

void TimedCall::Update() {
	if (IsFinish_) {
		return;
	}
	time_--;
	if (time_ <= 0) {
		IsFinish_ = true;
		f_();
	}
}
