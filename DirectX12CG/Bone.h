#pragma once
#include "Vector3D.h"
#include "Quaternion.h"
#include "MCBMatrix.h"
#include <vector>
#include <memory>
#include <string>

namespace MCBRef
{

	class Bone
	{

	private:
		MCB::Vector3D translation_;
		MCB::Vector3D scale_;
		MCB::Quaternion rotation_;

		Bone* parentBone_;

		std::vector<std::shared_ptr<Bone>> childBones_;

		MCB::MCBMatrix localMatrix_;

		MCB::MCBMatrix iKLocalMatrix_;

		MCB::MCBMatrix globalMatrix_;


		MCB::MCBMatrix inverseBindMatrix_;

		bool isIK;

		std::string name_;

		int32_t parentId_;

		void CalculateLocalMatrix();

		void CalculateGlobalMatrix();

	public:


		void SetParent(Bone* parent);

		void AddChild(Bone* bone);

		void SetRotation(MCB::Quaternion rotation);

		void SetTranslate(MCB::Vector3D translate);

		void SetScale(MCB::Vector3D scale);

		void SetInverseBindMatrix(MCB::MCBMatrix mat);

		void SetName(std::string name);

		MCB::MCBMatrix GetGlobalMatrix();

		MCB::MCBMatrix GetLocalMatrix();

		void SetIK(bool flag);

		Bone* GetParent();

	};
}