
#pragma once
#include "Object3d.h"
#include "SphereCollider.h"
#include "PopEffect.h"
#include "IgnoreWarning.h"
WarningIgnoreBegin
#include <vector>
WarningIgnoreEnd

namespace MCB
{
	class RotationBlock :public Object3d
	{
	public:
		float rotateSpeed = 0.025f;
		void UniqueUpdate()override;
		Byte6 pad1;
		Byte6 pad2;
	};
}
