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
public:
	MCB::Model* popModel_ = nullptr;
	static void ResetGoal() { goal_ = false; }
	void Init();
	void UpDate();
	void Draw()override;
	static bool GetIsGoal() { return goal_; };
	void OnCollision(const MCB::CollisionInfomation& info)override;
};

