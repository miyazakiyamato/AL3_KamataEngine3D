#pragma once
#include <stdio.h>
#include <functional>

class TimedCall {
public:
	TimedCall(std::function<void(void)> f, uint32_t time) : f_(f), time_(time) {}
	void Update();
	bool IsFinished() { return IsFinish_; }

private:
	//コールバック
	std::function<void(void)> f_;
	//
	uint32_t time_;
	bool IsFinish_ = false;
};
