#include "RayObject.h"

void MCB::RayObject::ColliderUpdate()
{
	collider.StartPosition.x = position.x - (collider.range / 2 * collider.rayVec.vec.x);
	collider.StartPosition.y = position.y - (collider.range / 2 * collider.rayVec.vec.y);
	collider.StartPosition.z = position.z - (collider.range / 2 * collider.rayVec.vec.z);
}

void MCB::RayObject::SetCollider(float rayRange, float rayRadius, Vector3D targetVec)
{
	collider.range = rayRange;
	collider.StartPosition.x = position.x - (collider.range / 2);
	collider.StartPosition.y = position.y - (collider.range / 2);
	collider.StartPosition.z = position.z - (collider.range / 2);
	collider.radius = rayRadius;
	collider.rayVec = targetVec;

}
