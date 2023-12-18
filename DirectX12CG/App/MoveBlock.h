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
		Vector3D moveVec = { 0,0,0 };
		Vector3D defaultPos = { 0,0,0 };
		float moveLength;
		float maxMoveLength = 3.5f;
		float speed = 0.025f;
		bool isSetMove = false;//上に乗った時だけ動くのかそうじゃないのか
		bool isMove = false;//動くかどうか
		int64_t pad;
		void UniqueUpdate()override;
	};
}

