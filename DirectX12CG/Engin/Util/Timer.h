#pragma once
class Timer
{
private:
	int timer = 0;
	int end = 0;

public:
	void Set(int end);
	void ReSet();
	void TimeSet(int timer, int end);
	void SetIf(int end, bool flag);
	void Update();
	void LoopUpdate();
	void SafeDownUpdate();
	void SafeUpdate();//�^�C�}�[��end�̐��l�𒴂��ăJ�E���g����Ȃ��悤�ɂȂ��Ă��܂��B
	int NowTime()const;
	bool IsEnd()const;
	int GetEndTime()const;
};