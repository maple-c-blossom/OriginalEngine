#include "SphereObj.h"

void MCB::SphereObj::SetCollider(float SphereRadius)
{
	centerPosition.x = position.x;
	centerPosition.y = position.y;
	centerPosition.z = position.z;
	radius = SphereRadius;
}

void MCB::SphereObj::ColliderUpdate()
{
	centerPosition.x = position.x;
	centerPosition.y = position.y;
	centerPosition.z = position.z;
}
