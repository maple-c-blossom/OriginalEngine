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
	class MoveBlock : public Object3d
	{
	public:
		Vector3D moveVec = { 0,0,1 };
		float moveLength;
		float maxMoveLength = 3.5f;
		float speed = 0.025f;
		int64_t pad;
		void UniqueUpdate()override;
	};
}

