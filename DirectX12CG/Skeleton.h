#pragma once
#include "Bone.h"

namespace MCBRef
{
	class Skeleton
	{
		std::vector<std::unique_ptr<Bone>> bones_;

		Bone* rootBone_;



	};
}

