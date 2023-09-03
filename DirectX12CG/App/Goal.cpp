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
	if (player->position_.z >= position_.z)
	{
		goal_ = true;
	}
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
}

void Goal::SetPlayer(MCB::Object3d* playerPtr)
{
	player = playerPtr;
}
