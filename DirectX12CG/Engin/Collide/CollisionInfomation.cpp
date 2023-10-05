#include "CollisionInfomation.h"

MCB::CollisionInfomation::CollisionInfomation(Object3d* object, BaseCollider* collider, const Vector3D& inter, float distance)
{
	object3d_ = object;
	collider_ = collider;
	inter_ = inter;
	distance_ = distance;
}