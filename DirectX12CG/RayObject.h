#pragma once
#include "Collider.h"
#include "Object3d.h"
namespace MCB
{
	class RayObject : public Object3d
	{
	public:
		Ray collider;
		
		void SetCollider(float rayRange,float rayRadius,Vector3D targetVec);
	};

}

