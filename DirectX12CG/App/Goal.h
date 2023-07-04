#pragma once
#include "Object3d.h"
#include "SphereCollider.h"

class Goal:public MCB::Object3d
{
private:
	static bool goal_;
public:
	static void ResetGoal() { goal_ = false; }
	void Init();
	static bool GetIsGoal() { return goal_; };
	void OnCollision(const MCB::CollisionInfomation& info)override;
};
