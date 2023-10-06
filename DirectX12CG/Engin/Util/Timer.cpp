#include "Timer.h"

void Timer::Set(int32_t end)
{
	this->end_ = end;
	timer_ = 0;
}

void Timer::ReSet()
{
	if (IsEnd())Set(end_);
}


void Timer::TimeSet(int32_t timer,int32_t end)
{
	this->timer_ = timer;
	this->end_ = end;
}

void Timer::SetIf(int32_t end, bool flag)
{
	if (flag)Set(end);
}

void Timer::Update()
{
	timer_++;
}

void Timer::LoopUpdate()
{
	Update();
	ReSet();
}

void Timer::SafeDownUpdate()
{
	if (timer_ > 0)
	{
		timer_--;
	}
}

void Timer::SafeUpdate()
{
	if (IsEnd())
	{
		return;
	}
	timer_++;
}

int Timer::NowTime() const
{
	return timer_;
}

bool Timer::IsEnd() const
{
	return timer_ >= end_;
}

int Timer::GetEndTime() const
{
	return end_;
}
