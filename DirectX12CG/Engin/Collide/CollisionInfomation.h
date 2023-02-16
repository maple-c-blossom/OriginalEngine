#pragma once
#include "Vector3D.h"

namespace MCB
{
	class Object3d;
	class BaseCollider;
	class CollisionInfomation
	{
	public:
		CollisionInfomation(Object3d* object, BaseCollider* collider, const Vector3D& inter,float distance = 0)
		{
			this->object3d = object;
			this->collider = collider;
			this->inter = inter;
			this->distance = distance;
		}
		Object3d* object3d = nullptr;
		BaseCollider* collider = nullptr;
		Vector3D inter;
		float distance;
	};

}