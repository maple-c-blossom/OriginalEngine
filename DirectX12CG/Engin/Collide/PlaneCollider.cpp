#include "PlaneCollider.h"

void MCB::PlaneCollider::Update()
{
	Plane::originDistance_ = originDistance_;
	Plane::normal_ = normal_;
	GetObject3D()->hited_ = false;
}
