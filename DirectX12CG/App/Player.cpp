#include "Player.h"
#include "Input.h"
#include "CollisionManager.h"
#include "PlayerQueryCallBack.h"
#include "AnimationModel.h"
using namespace std;
void MCB::Player::SetRespowPosition(const Vector3D& pos)
{
	respownPosition_ = pos;
}
void MCB::Player::Init()
{
	Object3d::Init();
	position_.z = -20;
	scale_ = { 1.25,1.25,1.25 };
	SetCollider(make_unique<SphereCollider>(Vector3D{0,0.5f,0},0.5f));
	collider_->SetAttribute(ATTRIBUTE_FLENDRY);
	UpdateMatrix();
	collider_->Update();
	nameId_ = "Player";
	position_ = { 0,0,-50 };
	respownPosition_ = position_;
	rotation_.y = ConvertRadius(180);
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
		\
		if (CollisionManager::GetInstance()->Raycast(ray, ATTRIBUTE_LANDSHAPE, &info, sphere->GetRaius() * 2.0f))
		{
			isGraund_ = true;
			position_.y -= (info.dist_ - sphere->GetRaius() * 2.0f);
			//animationModel_->TwoBoneIkOrder({ position_.x,position_.y,position_.z }, { position_.x,position_.y - sphere->GetRaius() / 2,position_.z });
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
		
		if (CollisionManager::GetInstance()->Raycast(ray, ATTRIBUTE_LANDSHAPE, &info, sphere->GetRaius() * 2.0f))
		{
			isGraund_ = true;
			position_.y -= (info.dist_ - sphere->GetRaius() * 2.0f);
			//animationModel_->TwoBoneIkOrder({ position_.x,position_.y,position_.z }, { position_.x,position_.y - sphere->GetRaius() / 2,position_.z });
			Object3d::UpdateMatrix();
		}
	}
	if (position_.y < outYPosition)
	{
		position_ = respownPosition_.ConvertXMFloat3();
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
		float accelerator = speed_ * 2;
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
		float accelerator = speed_ * 2;
		accelerator *= input_->gamePad_->LStick_.x_;
		if (speedRight_ <= maxspeed_ && speedRight_ >= -maxspeed_)speedRight_ += accelerator;
		else if (speedRight_ >= maxspeed_) speedRight_ = maxspeed_;
		else if (speedRight_ <= -maxspeed_)speedRight_ = -maxspeed_;
	}

	if (!input_->IsKeyDown(DIK_D) && !input_->IsKeyDown(DIK_A) && !input_->gamePad_->LStick_.x_)
	{
		speedRight_ = 0;
	}

	if (abs(speedFront_) > abs(speedRight_))
	{
		if (speedFront_ != 0)
		{
			if (speedFront_ > 0)currentAnimation_ = "Run";
			if (speedFront_ < 0)currentAnimation_ = "Run_Back";

		}
	}
	else if(speedRight_ != 0)
	{
		if (speedRight_ > 0)currentAnimation_ = "Run_Right";
		if (speedRight_ < 0)currentAnimation_ = "Run_Left";
	}
	else
	{
		animationSpeed_ = 0.05f;
		currentAnimation_ = "Idle";
	}
	animationSpeed_ = max(abs(speedFront_) / 7, abs(speedRight_) / 7);
	position_.x += nowFrontVec_.vec_.x_ * speedFront_;
	position_.z += nowFrontVec_.vec_.z_ * speedFront_;
	position_.x += rightVec_.vec_.x_ * speedRight_;
	position_.z += rightVec_.vec_.z_ * speedRight_;
	if (!isGraund_)
	{
		const float fallAcc = -0.03f;
		const float VYMin = -0.5f;
		fallV_.vec_.y_ = max(fallV_.vec_.y_ + fallAcc, VYMin);
		position_.x += fallV_.vec_.x_;
		position_.y += fallV_.vec_.y_;
		position_.z += fallV_.vec_.z_;
	}
	else if (Input::GetInstance()->IsKeyDown(DIK_SPACE) || input_->gamePad_->IsButtonDown(GAMEPAD_A))
	{
		isGraund_ = false;
		const float jumpVYFist = 0.45f;
		fallV_ = { 0,jumpVYFist,0,0 };
	}

}


