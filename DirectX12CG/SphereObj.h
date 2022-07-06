#pragma once
#include "Collider.h"
#include "Object3d.h"

namespace MCB
{
	class SphereObj:public Object3d,Sphere
	{
		void SetCollider(float rayRadius);
	};

}

