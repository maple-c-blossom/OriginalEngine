#pragma once
#include "Object3d.h"
#include "SphereCollider.h"
#include "PopEffect.h"
#include <vector>

class Goal:public MCB::Object3d
{
private:
	static bool goal_;
	std::list<std::unique_ptr<MCB::PopEffect>> effects_;
	int32_t timer_;
	MCB::Object3d* player;
public:
	MCB::Model* popModel_ = nullptr;
	static void ResetGoal() { goal_ = false; }
	void Init()override;
	void UniqueUpdate()override;
	void Draw()override;
	void DebugTextDraw(MCB::DebugText* debugText) override;
	int32_t GetTime() { return timer_; }
	static bool GetIsGoal() { return goal_; };
	void OnCollision(const MCB::CollisionInfomation& info)override;
	void SetPlayer(MCB::Object3d* playerPtr);
};

