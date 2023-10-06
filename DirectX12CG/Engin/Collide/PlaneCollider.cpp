#include "PlaneCollider.h"

MCB::PlaneCollider::PlaneCollider(const Vector3D& normal, float originDistance)
{
	normal_ = normal, originDistance_ = originDistance; primitive_ = PrimitiveType::PLANE;
};

void MCB::PlaneCollider::Update()
{
	Plane::originDistance_ = originDistance_;
	Plane::normal_ = normal_;
	GetObject3D()->hited_ = false;

}

 void MCB::PlaneCollider::SetDistance(float originDistance)
{
	originDistance_ = originDistance;
}