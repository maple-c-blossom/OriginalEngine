#include "BoxCollider.h"
void MCB::BoxCollider::Update()
{
	DirectX::XMMATRIX mat = object3d_->GetMatWorld();
	Box::position_.x_ = mat.r[3].m128_f32[0] + offset_.vec_.x_;
	Box::position_.y_ = mat.r[3].m128_f32[1] + offset_.vec_.y_;
	Box::position_.z_ = mat.r[3].m128_f32[2] + offset_.vec_.z_;
	Box::size_ = size_;
	GetObject3D()->hited_ = false;
}
