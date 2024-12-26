#pragma once
#include <stdint.h>
#include "Vector3.h"
#include "Quaternion.h"
#include "Bone.h"

namespace MCB
{
	class IKSolver
	{
	private:
		bool isLimit_;
		bool isCoordinateTransformation_;
		int32_t linkBoneCount_ = 4;
		float threshold_ = 1.0e-5f;
		int32_t iteration_ = 4;
		Vector3 targetPos_;
		Bone* effector_;

	public:

		void CCDIK();
		void SetIKConfig(int32_t linkBone, int32_t iteration,bool isLimit,
			bool isCoordinateTransformation,float threshold = 1.0e-5f);
		void SetTarget(Vector3 targetPos);

	};
}
