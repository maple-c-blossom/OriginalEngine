#include "IKSolver.h"
#include "MathUtil.h"

void MCB::IKSolver::CCDIK()
{
	bool run = true;
	bool remaining = false;
	int32_t iteration = iteration_;
	Bone* effectorB = effector_;
	Bone* effectorP = effectorB->GetParent();
	
	for (int32_t i = 0; i < linkBoneCount_; i++)
	{
		if (effectorP == nullptr)
		{
			break;
		}

		effectorB->SetModelTranslation(effectorB->GetInitializeModelTranslate());
		effectorP->SetModelTranslation(effectorP->GetInitializeModelTranslate());

		effectorB->SetRotation(effectorB->GetInitializeRotation());
		effectorP->SetRotation(effectorP->GetInitializeRotation());

		effectorB = effectorP;
		effectorP = effectorB->GetParent();
	}

	while (iteration-- && run)
	{
		Quaternion idealRotation;
		Quaternion realRotation;
		Quaternion remainingRotation;

		Vector3 localTargetPos = targetPos_;


		Bone* effectorBone = effector_;
		Bone* effectorParent = effectorBone->GetParent();
		Bone* rootBone = nullptr;

		Vector3 localEffectorPos = effectorBone->GetModelTranslate();
		Vector3 localEffectorParentPos = effectorParent->GetModelTranslate();

		for (int32_t i = 0; i < linkBoneCount_; i++)
		{

			localEffectorPos = effectorBone->GetModelTranslate();
			localEffectorParentPos = effectorParent->GetModelTranslate();

			//óùëzâÒì]çÏê¨
			Vector3 boneVec = Vector3(localEffectorParentPos, localEffectorPos);
			Vector3 effectToTarget = Vector3(localEffectorParentPos, localTargetPos);

			boneVec.V3Norm();
			effectToTarget.V3Norm();

			Vector3 axis = boneVec.GetV3Cross(effectToTarget);
			float dotRadian = effectToTarget.GetV3Dot(boneVec);
			float radian = MathUtil::Clamp(acos(dotRadian),-1,1);

			if (radian < threshold_)
			{
				continue;
			}

			idealRotation.SetRota(axis, radian);

			idealRotation = idealRotation.GetDirectProduct(idealRotation, effectorParent->GetRotation());

			if (remaining)
			{
				idealRotation = idealRotation.GetDirectProduct(idealRotation, remainingRotation);
			}

			if (isLimit_)
			{
				Vector3 eulerRot = Matrix::GetQuaternionRotaMat(idealRotation).GetRotationToEuler();

				eulerRot = eulerRot.Clamp(eulerRot, effectorParent->GetBottomLimitEulerRadian(),
					effectorParent->GetTopLimitEulerRadian(), &remaining);

				realRotation = realRotation.SetToRorateObjectToInternal(eulerRot);

				remainingRotation = realRotation.GetDirectProduct(
					realRotation.GetConjugated(realRotation), idealRotation);

			}
			else
			{
				realRotation = idealRotation;
			}
			realRotation.Normalize();
			effectorParent->SetRotation(realRotation);

			std::vector<Bone*> nodes;
			nodes.push_back(effectorParent);
			int32_t count = 0;
			while (count < nodes.size())
			{
				nodes[count]->UpdateMatrix();
				for (auto child : nodes[count]->GetChildren())
				{
					nodes.push_back(child);
				}
				count++;
			}
			

			effectorParent = effectorParent->GetParent();
			if (effectorParent == nullptr)
			{
				break;
			}
			else
			{
				rootBone = effectorParent;
			}
		}


		if (rootBone)
		{
			std::vector<Bone*> nodes;
			nodes.push_back(rootBone);
			int32_t count = 0;
			while (count < nodes.size())
			{
				nodes[count]->UpdateMatrix();
				for (auto child : nodes[count]->GetChildren())
				{
					nodes.push_back(child);
				}
				count++;
			}
		}
	}
}

void MCB::IKSolver::SetIKConfig(int32_t linkBone, int32_t iteration, bool isLimit, bool isCoordinateTransformation, float threshold)
{
	linkBoneCount_ = linkBone;
	iteration_ = iteration;
	isLimit_ = isLimit;
	isCoordinateTransformation_ = isCoordinateTransformation;
	threshold_ = threshold;

}

void MCB::IKSolver::SetTarget(Vector3 targetPos)
{
	targetPos_ = targetPos;
}
