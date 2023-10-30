#pragma once
#include "IgnoreWarning.h"
WarningIgnoreBegin
#include <cstdint>
WarningIgnoreEnd
class Timer
{
private:
	int32_t timer_ = 0;
	int32_t end_ = 0;
public:
	void Set(int32_t end);
	void ReSet();
	void TimeSet(int32_t timer,int32_t end);
	void SetIf(int32_t end, bool flag);
	void Update(int32_t count = 1);
	void LoopUpdate();
	void SafeDownUpdate();
	void SafeUpdate();//タイマーがendの数値を超えてカウントされないようになっています。
	int32_t NowTime()const;
	bool IsEnd()const;
	int32_t GetEndTime()const;
};