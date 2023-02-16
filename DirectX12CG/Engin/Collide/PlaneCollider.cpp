#include "PlaneCollider.h"

void MCB::PlaneCollider::Update()
{
	Plane::originDistance = originDistance;
	Plane::normal = normal;
	GetObject3D()->hited = false;
}
