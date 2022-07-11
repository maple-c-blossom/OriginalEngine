#pragma once
#include "Collider.h"
#include "Object3d.h"

namespace MCB
{
	class SphereObj:public Object3d, public Sphere
	{
	public:
		void SetCollider(float SphereRadius);

		void ColliderUpdate();
	};

}

