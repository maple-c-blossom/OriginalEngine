#pragma once
#include "BaseCollider.h"
#include "Vector3d.h"
namespace MCB
{
	class Object3d;
	class RayCastHit
	{
	public:
		Object3d* objctPtr = nullptr;
		BaseCollider* collPtr = nullptr;
		Vector3D inter;
		float dist;
	};

}

