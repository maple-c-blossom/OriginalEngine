#include "Goal.h"
#include "DxWindow.h"
#include "DebugText.h"
using namespace MCB;
using namespace std;
bool Goal::goal_ = false;
void Goal::Init()
{
	Object3d::Init();
	goal_ = false;
	timer_ = 0;
	SetCollider(make_unique<SphereCollider>(Vector3D{0,0,0},2.f));
	collider_->SetAttribute(ATTRIBUTE_ENEMY);
	UpdateMatrix();
	collider_->Update();
}

void Goal::UniqueUpdate()
{
	if (!goal_)
	{
		timer_++;
	}

	for (auto& effect : effects_)
	{
		effect->UniqueUpdate();
		effect->UpdateMatrix(camera_);
	}
	effects_.remove_if([](auto& effect) {return effect->deleteFlag_; });
	
}

void Goal::Draw()
{
	if(!goal_)Object3d::Draw();
	for (auto& effect : effects_)
	{
		effect->Draw();
	}
}

void Goal::DebugTextDraw(MCB::DebugText* debugText)
{

	if (Goal::goal_)
	{
		debugText->Print(DxWindow::GetInstance()->sWINDOW_WIDTH_ / 2.f, DxWindow::GetInstance()->sWINDOW_HEIGHT_ / 2.f, 5,
			"Goal!!!");
		debugText->Print(DxWindow::GetInstance()->sWINDOW_WIDTH_ / 2.f, DxWindow::GetInstance()->sWINDOW_HEIGHT_ / 2.f + 120.f, 
			3, "Press AButton");
	}
	debugText->Print(DxWindow::GetInstance()->sWINDOW_WIDTH_ - 200.f, 20.f, 2, "TIME:%d", timer_ / 60);
}

void Goal::OnCollision(const MCB::CollisionInfomation& info)
{
	hited_ = true;
	std::string hitName = info.object3d_->nameId_;
	if (hitName == "player")
	{
		if (!goal_)
		{
			for (int i = 0; i < 10; i++)
			{
				unique_ptr<PopEffect> effect = make_unique<PopEffect>();
				effect->Initialize(popModel_, { sinf(ConvertRadius((float)GetRand(0,360))) * cosf(ConvertRadius((float)GetRand(0,360))),sinf(ConvertRadius((float)GetRand(0,360))) * sinf(ConvertRadius((float)GetRand(0,360))),cosf(ConvertRadius((float)GetRand(0,360))) },
					{ position_.x + GetRand(0,200) / 100,position_.y + GetRand(0,200) / 100,position_.z + GetRand(0,200) / 100 }, { (float)0.25,(float)0.25,(float)0.25 }, { 1,0,0,1 }, 0.25f, 30);
				effects_.push_back(std::move(effect));
			}
		}
		goal_ = true;
	}
	sceneEnd = goal_;
}
