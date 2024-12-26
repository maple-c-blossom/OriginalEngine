#include "Skelton.h"
#include "Vector3.h"
void MCB::Skelton::InitializeCapture()
{
	capture = std::make_unique<Capture>();
	capture->Initialize();
	capture->SetInitialPose();
}

void MCB::Skelton::AddBone(std::unique_ptr<Bone> bone)
{
	bones_.push_back(std::move(bone));
}

MCB::Bone* MCB::Skelton::GetBone(std::string name)
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

MCB::Quaternion MCB::Skelton::GetBoneRotation(std::string name)
{
	return GetBone(name)->GetRotation();
}

void MCB::Skelton::UpdateCaptureData()
{
	capture->Update();
}

void MCB::Skelton::CaptureBasePoseInitialize()
{
	capture->SetInitialPose();

}

void MCB::Skelton::SetRootBone(Bone* bone)
{
	rootBone_ = bone;
}

void MCB::Skelton::CaptureBoneAccept()
{



}

void MCB::Skelton::CaptureBoneUpdate(YOLO_POSE_INDEX rootBoneName, uint32_t boneCount)
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



