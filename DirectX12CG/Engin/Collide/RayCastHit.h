#pragma once
#include "BaseCollider.h"
#include "Vector3d.h"
namespace MCB
{
	class Object3d;
	class RayCastHit
	{
	public:
		Object3d* objctPtr_ = nullptr;
		BaseCollider* collPtr_ = nullptr;
		Vector3D inter_;
		float dist_;
	};

}

