#pragma once
#include "Bone.h"
#include <unordered_map>
#include "IKSolver.h"

namespace MCBRef
{

	struct SetBoneData
	{
		std::string name;
		MCB::Vector3D translation_;
		MCB::Vector3D scale_;
		MCB::Quaternion rotation_;
		Bone* parentBone_;


	};

	class Skeleton
	{
	private:
		std::unordered_map<std::string,std::unique_ptr<Bone>> bones_;
		std::unordered_map<std::string,std::unique_ptr<IKSolver>> ikSolvers_;

		Bone* rootBone_;

	public:
		void SetBone(SetBoneData boneData);
	};
}

