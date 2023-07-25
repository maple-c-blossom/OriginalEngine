#include "PopEffect.h"
using namespace MCB;
void PopEffect::Initialize(MCB::Model* model, MCB::Vector3D Velocity, MCB::Float3 position, MCB::Float3 scale, MCB::Float4 color, float speed, int lifeTime)
{
	model_ = model;;
	position_.x = position.x_;
	position_.y = position.y_;
	position_.z = position.z_;
	defaultScale_.x_ = scale.x_;
	defaultScale_.y_ = scale.y_;
	defaultScale_.z_ = scale.z_;
	defaultSpeed_ = speed;
	lifeTimer_.Set(lifeTime);
	Object3d::Init();
	nowFrontVec_.vec_.x_ = Velocity.vec_.x_;
	nowFrontVec_.vec_.y_ = Velocity.vec_.y_;
	nowFrontVec_.vec_.z_ = Velocity.vec_.z_;
	nowFrontVec_.V3Norm();
	this->color_ = color;
}

void PopEffect::UniqueUpdate()
{
	lifeTimer_.SafeUpdate();
	speed_ = static_cast<float>(InQuad(defaultSpeed_, 0, lifeTimer_.GetEndTime(), lifeTimer_.NowTime()));
	scale_.x = static_cast<float>(InQuad(defaultScale_.x_, 0, lifeTimer_.GetEndTime(), lifeTimer_.NowTime()));
	scale_.y = static_cast<float>(InQuad(defaultScale_.y_, 0, lifeTimer_.GetEndTime(), lifeTimer_.NowTime()));
	scale_.z = static_cast<float>(InQuad(defaultScale_.z_, 0, lifeTimer_.GetEndTime(), lifeTimer_.NowTime()));
	color_.w_ = static_cast<float>(InQuad(1, 0, lifeTimer_.GetEndTime(), lifeTimer_.NowTime()));
	position_.x += nowFrontVec_.vec_.x_ * speed_;
	position_.y += nowFrontVec_.vec_.y_ * speed_;
	position_.z += nowFrontVec_.vec_.z_ * speed_;
	//rotation.x_ += ConvertRadius(speed);
	if (lifeTimer_.IsEnd())
	{
		deleteFlag_ = true;
	}
}

void PopEffect::UpdateMatrix(MCB::ICamera* camera)
{
	Object3d::camera_ = camera;
	Object3d::Update();
}

void PopEffect::Draw()
{
	Object3d::Draw();
}
