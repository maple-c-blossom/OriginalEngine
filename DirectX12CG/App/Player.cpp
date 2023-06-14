#include "Player.h"
#include "Input.h"
#include "CollisionManager.h"
using namespace std;
void MCB::Player::Init()
{
	Object3d::Init();
	SetCollider(make_shared<SphereCollider>());
	collider_->SetAttribute(ATTRIBUTE_ENEMY);
}

void MCB::Player::Update()
{


	if (Input::GetInstance()->IsKeyDown(DIK_A))
	{
		position_.x -= 0.05f;
	}
	if (Input::GetInstance()->IsKeyDown(DIK_D))
	{
		position_.x += 0.05f;
	}
	if (Input::GetInstance()->IsKeyDown(DIK_W))
	{
		position_.z += 0.1f;
	}
	if (Input::GetInstance()->IsKeyDown(DIK_S))
	{
		position_.z -= 0.1f;
	}
	if (!isGraund_)
	{
		const float fallAcc = -0.01f;
		const float VYMin = -0.5f;
		fallV_.vec_.y_ = max(fallV_.vec_.y_ + fallAcc, VYMin);
		position_.x += fallV_.vec_.x_;
		position_.y += fallV_.vec_.y_;
		position_.z += fallV_.vec_.z_;
	}
	else if (Input::GetInstance()->IsKeyTrigger(DIK_SPACE))
	{
		isGraund_ = false;
		const float jumpVYFist = 0.2f;
		fallV_ = { 0,jumpVYFist,0,0};
	}

	Object3d::Update();
	SphereCollider* sphere = dynamic_cast<SphereCollider*>(collider_.get());
	assert(sphere);

	Ray ray;
	ray.StartPosition_ = sphere->centerPosition_;
	ray.StartPosition_.vec_.y_ += sphere->GetRaius();
	ray.rayVec_ = { 0,-1,0,0 };
	RayCastHit info;
	if (isGraund_)
	{
		const float absDistance = 0.2f;
		if (CollisionManager::GetInstance()->Raycast(ray, ATTRIBUTE_LANDSHAPE, &info, sphere->GetRaius() * 2.0f))
		{
			isGraund_ = true;
			position_.y -= (info.dist_ - sphere->GetRaius() * 2.0f);
			Object3d::Update();
		}
		else
		{
			isGraund_ = false;
			fallV_ = {0,0,0,0};
		}
	}
	else if(fallV_.vec_.y_ <= 0.0f)
	{
		if (CollisionManager::GetInstance()->Raycast(ray, ATTRIBUTE_LANDSHAPE, &info, sphere->GetRaius() * 2.0f))
		{
			isGraund_ = true;
			position_.y -= (info.dist_ - sphere->GetRaius() * 2.0f);
			Object3d::Update();
		}
	}
}




void MCB::Player::Draw()
{
	Object3d::Draw();
}
