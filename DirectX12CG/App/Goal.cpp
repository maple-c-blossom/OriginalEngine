#include "Goal.h"

using namespace MCB;
using namespace std;
bool Goal::goal_ = false;
void Goal::Init()
{
	Object3d::Init();
	goal_ = false;
	SetCollider(make_unique<SphereCollider>(Vector3D{0,0,0},2.f));
	collider_->SetAttribute(ATTRIBUTE_ENEMY);
	UpdateMatrix();
	collider_->Update();
}

void Goal::OnCollision(const MCB::CollisionInfomation& info)
{
	hited_ = true;
	std::string hitName = info.object3d_->nameId_;
	if (hitName == "Player")
	{
		goal_ = true;
	}
}
