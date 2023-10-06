#include "BaseCollider.h"

void MCB::BaseCollider::SetObject(Object3d* object)
{
	object3d_ = object;
}

MCB::PrimitiveType MCB::BaseCollider::GetPrimitive()
{
	return primitive_;
}

void MCB::BaseCollider::OnCollision(const CollisionInfomation& info)
{
	object3d_->OnCollision(info);
}

void MCB::BaseCollider::OffCollision(const CollisionInfomation& info)
{
	object3d_->OffCollision(&info);
}

void MCB::BaseCollider::SetAttribute(uint16_t attribute)
{
	attribute_ = attribute;
}

void MCB::BaseCollider::AddAttribute(uint16_t attribute)
{
	attribute_ |= attribute;
}

void MCB::BaseCollider::RemoveAttribute(uint16_t attribute)
{
	attribute_ &= !attribute;
}

MCB::Object3d* MCB::BaseCollider::GetObject3D()
{
	return object3d_;
}
