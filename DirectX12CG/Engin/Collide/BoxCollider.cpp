#include "BoxCollider.h"
MCB::BoxCollider::BoxCollider(const Vector3D& offset, Float3 size)
{
	offset_ = offset, size_ = size; primitive_ = PrimitiveType::BOX;
}
void MCB::BoxCollider::Update()
{
	DirectX::XMMATRIX mat = object3d_->GetMatWorld();
	Box::position_.x_ = mat.r[3].m128_f32[0] + offset_.vec_.x_;
	Box::position_.y_ = mat.r[3].m128_f32[1] + offset_.vec_.y_;
	Box::position_.z_ = mat.r[3].m128_f32[2] + offset_.vec_.z_;
	Box::size_ = size_;
	GetObject3D()->hited_ = false;
}

void MCB::BoxCollider::SetSize(const Float3& size)
{
	size_ = size;
}