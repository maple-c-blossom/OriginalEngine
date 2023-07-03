#include "RayCollider.h"

void MCB::RayCollider::Update()
{
	DirectX::XMMATRIX mat = object3d_->GetMatWorld();
	Ray::StartPosition_.vec_.x_ = mat.r[3].m128_f32[0] + offset_.vec_.x_;
	Ray::StartPosition_.vec_.y_ = mat.r[3].m128_f32[1] + offset_.vec_.y_;
	Ray::StartPosition_.vec_.z_ = mat.r[3].m128_f32[2] + offset_.vec_.z_;
	Ray::rayVec_.vec_.x_ = rayVec_.vec_.x_;
	Ray::rayVec_.vec_.y_ = rayVec_.vec_.y_;
	Ray::rayVec_.vec_.z_ = rayVec_.vec_.z_;
	Ray::radius_ = radius_;
	Ray::range_ = range_;
	Ray::rayCasted_ = false;
	GetObject3D()->hited_ = false;
}
