#pragma once
#include <stdint.h>
#include <Math/Vector3.h>
#include <Math/Quaternion.h>
#include <Animation/Bone.h>

namespace MCBM
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
