#pragma once
#include "IgnoreWarning.h"
WarningIgnoreBegin
#include <cstdint>
WarningIgnoreEnd
class Timer
{
private:
	int timer_ = 0;
	int end_ = 0;
public:
	void Set(int end);
	void ReSet();
	void TimeSet(int timer, int end);
	void SetIf(int end, bool flag);
	void Update();
	void LoopUpdate();
	void SafeDownUpdate();
	void SafeUpdate();//タイマーがendの数値を超えてカウントされないようになっています。
	int NowTime()const;
	bool IsEnd()const;
	int GetEndTime()const;
};