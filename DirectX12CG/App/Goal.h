#pragma once
#include "Object3d.h"
#include "SphereCollider.h"
#include "PopEffect.h"
#include "IgnoreWarning.h"
#include "Sprite.h"
WarningIgnoreBegin
#include <vector>
WarningIgnoreEnd
class Goal:public MCB::Object3d
{
private:
	static bool goal_;
	std::list<std::unique_ptr<MCB::PopEffect>> effects_;
	int32_t timer_;
	Byte4 pad;
	MCB::Object3d* player;
	std::array<MCB::Sprite,3> numSprite;
	MCB::Sprite timeSprite;
	MCB::Sprite goalSprite;
	MCB::Sprite pressSprite;
	MCB::TextureCell* numTex_;
	MCB::TextureCell* timeTex_;
	MCB::TextureCell* goalTex_;
	MCB::TextureCell* pressTex_;
public:
	MCB::Model* popModel_ = nullptr;
	static void ResetGoal();
	void Init()override;
	void UniqueUpdate()override;
	void Draw()override;
	void Draw(uint16_t incremant)override;
	void DebugTextDraw(MCB::DebugText* debugText) override;
	int32_t GetTime();
	static bool GetIsGoal();
	void OnCollision(const MCB::CollisionInfomation& info)override;
	void SetPlayer(MCB::Object3d* playerPtr);
};

