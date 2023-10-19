#include "Player.h"
#include "Input.h"
#include "CollisionManager.h"
#include "PlayerQueryCallBack.h"
#include "AnimationModel.h"
#include "Sound.h"

using namespace std;
float MCB::Player::GetSpeed()
{
	return speedFront_;

}
void MCB::Player::SetRespowPosition(const Vector3D& pos)
{
	respownPosition_ = pos;
}
void MCB::Player::Init()
{
	Object3d::Init();
	scale_ = { 0.01f,0.01f,0.01f };
	SetCollider(make_unique<SphereCollider>(Vector3D{0,0.5f,0},0.5f));
	collider_->SetAttribute(ATTRIBUTE_FLENDRY);
	UpdateMatrix();
	collider_->Update();
	//position_ = { 0,0,-50 };
	respownPosition_ = position_;
	rotation_.y = ConvertRadius(180);
	rotation_.x = ConvertRadius(0);

}

void MCB::Player::UniqueUpdate()
{
	if (!back)
	{
		Move();
	}

	Object3d::UpdateMatrix();
	SphereCollider* sphere = dynamic_cast<SphereCollider*>(collider_);
	assert(sphere);


	Ray ray;
	ray.StartPosition_ = sphere->centerPosition_;
	ray.StartPosition_.vec_.y_ += sphere->GetRaius();
	ray.rayVec_ = { {{0,-1,0,0}} };
	RayCastHit info;
	float distRange = sphere->GetRaius() * 2.0f;
	float distOverRange = distRange * distoffSet;
	bool isGround = CollisionManager::GetInstance()->Raycast(ray, ATTRIBUTE_LANDSHAPE, &info, distRange + distOverRange);
	if (isGraund_)
	{
		//const float absDistance = 0.2f;
		
		if (isGround)
		{
			isGraund_ = true;
			if(info.dist_ <= 1.f - distOverRange) position_.y -= ((info.dist_) - sphere->GetRaius() * 2.0f);
			Object3d::UpdateMatrix();
			for ( uint8_t i = 0; i < footBoneName.size(); i++ )
			{
				animationModel_->skeleton.GetNode(footBoneName[ i ])->ikData.isCollisionIk = true;
				//animationModel_->skeleton.SetTwoBoneIK();
			}
		}
		else
		{
			isGraund_ = false;
			fallV_ = { {{0,0,0,0}} };
		}
	}
	else if(fallV_.vec_.y_ <= 0.0f)
	{
		
		if ( isGround )
		{
			isGraund_ = true;
			if (info.dist_ <= 1.f - distOverRange) position_.y -= ((info.dist_ - distOverRange) - sphere->GetRaius() * 2.0f);
			Object3d::UpdateMatrix();
		}
		else 
		{
			isGraund_ = false;
		}
	}

	PlayerQueryCallBack callback(sphere);

	CollisionManager::GetInstance()->QuerySphere(*sphere, &callback, ATTRIBUTE_LANDSHAPE);

	if (callback.move.V3Len() >= distoffSet)
	{
		position_.x += callback.move.vec_.x_ * 2;
		position_.y += callback.move.vec_.y_ * 2;
		position_.z += callback.move.vec_.z_ * 2;
	}

	if (back)
	{
		backTimer.SafeUpdate();
		position_.x += backVec.vec_.x_;
		position_.y += backVec.vec_.y_;
		position_.z += backVec.vec_.z_;
		if (backTimer.IsEnd())
		{
			back = false;
		}
	}
	else
	{
		backTimer.Set(5);
		backVec = callback.move * 6;
		back = callback.block;
	}

	if (position_.y < outYPosition)
	{
		position_ = respownPosition_.ConvertXMFloat3();
	}
}


void MCB::Player::Move()
{
	//SoundManager* sm = SoundManager::GetInstance();
	if (!input_->IsKeyDown(DIK_S) && !input_->IsKeyDown(DIK_W) && 
		!input_->gamePad_->RTrriger_.x_&& !input_->gamePad_->LTrriger_.x_)
	{
		speedFront_ = defualtSpeed_;
	}

	if (input_->IsKeyDown(DIK_W))
	{
		speedFront_ += acceleratorfront_;
	}

	if (input_->IsKeyDown(DIK_S))
	{
		speedFront_ -= acceleratorfront_;
	}

	if (input_->gamePad_->RTrriger_.x_)
	{
		accelerator_ *= input_->gamePad_->RTrriger_.x_;
		speedFront_ += acceleratorfront_;
	}
	else if(input_->gamePad_->LTrriger_.x_)
	{
		accelerator_ *= input_->gamePad_->LTrriger_.x_;
		speedFront_ -= acceleratorfront_;
	}

	speedFront_ = clamp(speedFront_, 0.0025f, maxFrontSpeed_);




	if (input_->IsKeyDown(DIK_D))
	{
		if (speedRight_ <= maxspeed_)speedRight_ += accelerator_;
		else speedRight_ = maxspeed_;
	}

	if (input_->IsKeyDown(DIK_A))
	{
		if (speedRight_ >= -maxspeed_)speedRight_ -= accelerator_;
		else speedRight_ = -maxspeed_;
	}

	if (input_->gamePad_->LStick_.x_)
	{
		float accelerator = maxspeed_;
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
			if (speedFront_ > 0)currentAnimation_ = "Run.001";
			if (speedFront_ < 0)currentAnimation_ = "Run.001";

		}
	}
	else if(speedRight_ != 0)
	{
		if (speedRight_ > 0)currentAnimation_ = "Run.001";
		if (speedRight_ < 0)currentAnimation_ = "Run.001";
	}
	else
	{
		animationSpeed_ = 0.05f;
		currentAnimation_ = "Tpose.001";
	}
	animationSpeed_ = max(abs(speedFront_) / 7, abs(speedRight_) / 7);
	position_.x += nowFrontVec_.vec_.x_ * speedFront_;
	position_.z += nowFrontVec_.vec_.z_ * speedFront_;
	position_.x += rightVec_.vec_.x_ * speedRight_;
	position_.z += rightVec_.vec_.z_ * speedRight_;
	if (!isGraund_)
	{
		const float fallAcc = -0.025f;
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
		fallV_ = { {{0,jumpVYFist,0,0}} };
	}

}

void MCB::Player::OnCollision(const CollisionInfomation& info)
{
	if (info.object3d_->nameId_ == "checkPoint")
	{
		respownPosition_ = info.object3d_->position_;
	}

}


