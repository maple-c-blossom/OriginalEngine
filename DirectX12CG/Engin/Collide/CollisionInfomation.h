#pragma once
#include "Vector3D.h"

namespace MCB
{
	class Object3d;
	class BaseCollider;
	class CollisionInfomation
	{
	public:
		CollisionInfomation(Object3d* object, BaseCollider* collider, const Vector3D& inter, float distance = 0)
		{
			object3d_ = object;
			collider_ = collider;
			inter_ = inter;
			distance_ = distance;
		}
		Object3d* object3d_ = nullptr;
		BaseCollider* collider_ = nullptr;
		Vector3D inter_;
		float distance_;
	};

}