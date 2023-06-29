#include "Player.h"
#include "Input.h"
#include "CollisionManager.h"
#include "PlayerQueryCallBack.h"
using namespace std;
void MCB::Player::Init()
{
	Object3d::Init();
	position_.z = -20;
	SetCollider(make_unique<SphereCollider>());
	collider_->SetAttribute(ATTRIBUTE_ENEMY);
}

void MCB::Player::UniqueUpdate()
{
	Move();

	Object3d::UpdateMatrix();
	SphereCollider* sphere = dynamic_cast<SphereCollider*>(collider_);
	assert(sphere);

	PlayerQueryCallBack callback(sphere);

	CollisionManager::GetInstance()->QuerySphere(*sphere, &callback, ATTRIBUTE_LANDSHAPE);
	position_.x += callback.move.vec_.x_;
	position_.y += callback.move.vec_.y_;
	position_.z += callback.move.vec_.z_;
	Ray ray;
	ray.StartPosition_ = sphere->centerPosition_;
	ray.StartPosition_.vec_.y_ += sphere->GetRaius();
	ray.rayVec_ = { 0,-1,0,0 };
	RayCastHit info;
	if (isGraund_)
	{
		const float absDistance = 0.2f;
		OutputDebugStringW(L"playerisGraund--------------------------------------------------\n");
		if (CollisionManager::GetInstance()->Raycast(ray, ATTRIBUTE_LANDSHAPE, &info, sphere->GetRaius() * 2.0f))
		{
			isGraund_ = true;
			position_.y -= (info.dist_ - sphere->GetRaius() * 2.0f);
			Object3d::UpdateMatrix();
		}
		else
		{
			isGraund_ = false;
			fallV_ = {0,0,0,0};
		}
	}
	else if(fallV_.vec_.y_ <= 0.0f)
	{
		OutputDebugStringW(L"playerNoisGraund--------------------------------------------------\n");
		if (CollisionManager::GetInstance()->Raycast(ray, ATTRIBUTE_LANDSHAPE, &info, sphere->GetRaius() * 2.0f))
		{
			isGraund_ = true;
			position_.y -= (info.dist_ - sphere->GetRaius() * 2.0f);
			Object3d::UpdateMatrix();
		}
	}

	if (position_.y <= -20)
	{
		position_.y = 0;
	}
}

void MCB::Player::Move()
{

	if (input_->IsKeyDown(DIK_W))
	{
		if (speedFront_ <= maxspeed_)speedFront_ += speed_;
		else speedFront_ = maxspeed_;
	}

	if (input_->IsKeyDown(DIK_S))
	{
		if (speedFront_ >= -maxspeed_)speedFront_ -= speed_;
		else speedFront_ = -maxspeed_;
	}

	if (input_->gamePad_->LStick_.y_)
	{
		float accelerator = speed_;
		accelerator *= input_->gamePad_->LStick_.y_;
		if (speedFront_ <= maxspeed_ && speedFront_ >= -maxspeed_)speedFront_ += accelerator;
		else if (speedFront_ >= maxspeed_) speedFront_ = maxspeed_;
		else if (speedFront_ <= -maxspeed_)speedFront_ = -maxspeed_;
	}

	if (!input_->IsKeyDown(DIK_S) && !input_->IsKeyDown(DIK_W) && !input_->gamePad_->LStick_.y_)
	{
		speedFront_ = 0;
	}


	if (input_->IsKeyDown(DIK_D))
	{
		if (speedRight_ <= maxspeed_)speedRight_ += speed_;
		else speedRight_ = maxspeed_;
	}

	if (input_->IsKeyDown(DIK_A))
	{
		if (speedRight_ >= -maxspeed_)speedRight_ -= speed_;
		else speedRight_ = -maxspeed_;
	}

	if (input_->gamePad_->LStick_.x_)
	{
		float accelerator = speed_;
		accelerator *= input_->gamePad_->LStick_.x_;
		if (speedRight_ <= maxspeed_ && speedRight_ >= -maxspeed_)speedRight_ += accelerator;
		else if (speedRight_ >= maxspeed_) speedRight_ = maxspeed_;
		else if (speedRight_ <= -maxspeed_)speedRight_ = -maxspeed_;
	}

	if (!input_->IsKeyDown(DIK_D) && !input_->IsKeyDown(DIK_A) && !input_->gamePad_->LStick_.x_)
	{
		speedRight_ = 0;
	}

	position_.x += nowFrontVec_.vec_.x_ * speedFront_;
	position_.z += nowFrontVec_.vec_.z_ * speedFront_;
	position_.x += rightVec_.vec_.x_ * speedRight_;
	position_.z += rightVec_.vec_.z_ * speedRight_;
	if (!isGraund_)
	{
		const float fallAcc = -0.01f;
		const float VYMin = -0.5f;
		fallV_.vec_.y_ = max(fallV_.vec_.y_ + fallAcc, VYMin);
		position_.x += fallV_.vec_.x_;
		position_.y += fallV_.vec_.y_;
		position_.z += fallV_.vec_.z_;
	}
	else if (Input::GetInstance()->IsKeyDown(DIK_SPACE) || input_->gamePad_->IsButtonDown(GAMEPAD_A))
	{
		isGraund_ = false;
		const float jumpVYFist = 0.2f;
		fallV_ = { 0,jumpVYFist,0,0 };
	}

}

