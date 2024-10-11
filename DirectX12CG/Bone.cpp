#include "Bone.h"

using namespace MCB;

void MCBRef::Bone::CalculateLocalMatrix()
{
	MCBMatrix tmat = MCBMatrix::MCBMatrixTranslate(translation_);
	MCBMatrix smat = MCBMatrix::MCBMatrixScaling(scale_);
	MCBMatrix rmat = rotation_.GetQuaternionRotaMat(rotation_);

	localMatrix_ = smat * rmat * tmat;

}

void MCBRef::Bone::CalculateGlobalMatrix()
{
	globalMatrix_ = localMatrix_ * parentBone_->GetGlobalMatrix();
}

MCBMatrix MCBRef::Bone::GetGlobalMatrix()
{
	return globalMatrix_;
}

MCBMatrix MCBRef::Bone::GetLocalMatrix()
{
	return localMatrix_;
}

void MCBRef::Bone::SetIK(bool flag)
{
	isIK = flag;
}

void MCBRef::Bone::SetRotation(Quaternion rotation)
{
	rotation_ = rotation;
}

void MCBRef::Bone::SetTranslate(Vector3D translate)
{
	translation_ = translate;
}

void MCBRef::Bone::SetScale(Vector3D scale)
{
	scale_ = scale;
}

void MCBRef::Bone::SetParent(Bone* parent)
{
	parentBone_ = parent;
}
