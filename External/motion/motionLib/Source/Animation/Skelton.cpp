#include <Animation/Skelton.h>
#include <Math/Vector3.h>

void MCBM::Skelton::SetCapturePtr(Capture* cap)
{
	capture = cap;
}

void MCBM::Skelton::AddBone(std::unique_ptr<Bone> bone)
{
	bones_.push_back(std::move(bone));
}

MCBM::Bone* MCBM::Skelton::GetBone(std::string name)
{
	for (auto& bone : bones_)
	{
		if (bone->GetName() == name)
		{
			return bone.get();
		}
	}
	return nullptr;
}

MCBM::Quaternion MCBM::Skelton::GetBoneRotation(std::string name)
{
	return GetBone(name)->GetRotation();
}

void MCBM::Skelton::UpdateCaptureData()
{
	capture->Update();
}

void MCBM::Skelton::CaptureBasePoseInitialize()
{
	capture->SetInitialPose();

}

void MCBM::Skelton::SetRootBone(Bone* bone)
{
	rootBone_ = bone;
}

void MCBM::Skelton::CaptureBoneAccept()
{



}

void MCBM::Skelton::Finalize()
{
	capture->Finalize();
}

void MCBM::Skelton::CaptureBoneUpdate(YOLO_POSE_INDEX rootBoneName, uint32_t boneCount)
{

	CaptureData rootCap = capture->GetCaptureData(rootBoneName);
	Vector3 tempVec;
	for (int i = 0; i < boneCount; i++)
	{
		Bone* rootBone = GetBone(rootCap.captureBoneName);
		for (int k = 0; k < 1; k++)
		{
			CaptureData* child = rootCap.captureChildren[k];
			Vector3 initializeBone = Vector3(rootCap.initializedCaptureBonePos, child->initializedCaptureBonePos);
			Vector3 nowBone = Vector3(rootCap.captureBonePos, child->captureBonePos);

			initializeBone.V3Norm();

			nowBone.V3Norm();

			if (i != 0)
			{
				initializeBone = tempVec;
			}

			tempVec = nowBone;

			Vector3 axis = nowBone.GetV3Cross(initializeBone);
			float dotRadian = nowBone.GetV3Dot(initializeBone);
			float rotation = acos(dotRadian);

			if (!isfinite(rotation))
			{
				rotation = 0;
			}
			axis.V3Norm();

			Quaternion q(axis, rotation);
			q.Normalize();
			rootBone->SetRotation(rootBone->GetInitializeRotation());
			Quaternion temp = q.GetDirectProduct(rootBone->GetRotation(), q);
			temp.Normalize();
			rootBone->SetRotation(temp);
		}
		rootCap = *rootCap.captureChildren[0];


	}
}



