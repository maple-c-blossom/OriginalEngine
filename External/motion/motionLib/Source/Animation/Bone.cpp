#include "Bone.h"

using namespace MCB;

void MCB::Bone::SetRotation(const Quaternion& rot)
{
	rotation_ = rot;
}

void MCB::Bone::SetName(const std::string& name)
{
	name_ = name;
}

void MCB::Bone::SetScale(const Vector3& scale)
{
	scale_ = scale;
}

void MCB::Bone::SetTranslation(const Vector3& translation)
{
	translation_ = translation;
}

void MCB::Bone::SetModelTranslation(const Vector3& translation)
{
	modelTranslate_ = translation;
}

void MCB::Bone::SetParent(Bone* parent)
{
	parent_ = parent;
}

void MCB::Bone::SetBoneRotationLimit(Vector3 topEulerLimit, Vector3 bottomEulerLimit)
{
	topLimitEulerRadian_ = topEulerLimit;
	bottomLimitEulerRadian_ = bottomEulerLimit;
}

std::string MCB::Bone::GetName()
{
	return name_;
}

Vector3 MCB::Bone::GetScale()
{
	return scale_;
}

Vector3 MCB::Bone::GetTranslation()
{
	return translation_;
}

Matrix MCB::Bone::GetOffSetMatrix()
{
	return offsetMatrix_;
}

Matrix MCB::Bone::GetFinalMatrix()
{
	return finalMatrix_;
}

void MCB::Bone::AddChild(Bone* child)
{
	children_.push_back(child);
}

void MCB::Bone::SetFinalMatrix(const Matrix& matrix)
{
	finalMatrix_ = matrix;
}

void MCB::Bone::SetOffsetMatrix(const Matrix& matrix)
{
	offsetMatrix_ = matrix;
}

void MCB::Bone::RemoveChild(Bone* child)
{
	auto itr = find(children_.begin(), children_.end(), child);
	if (itr != children_.end())
	{
		children_.erase(itr);
	}

}

void MCB::Bone::SetParentAndChild(Bone* parent)
{
	if (parent_ != nullptr)
	{
		parent_->RemoveChild(this);
	}
	parent_ = parent;
	parent_->AddChild(this);
}

void MCB::Bone::SetInitializeTransformData()
{
	initializeLocalTranslation_ = translation_;
	initializeRotation_ = rotation_;
	initializeScale_ = scale_;
	UpdateMatrix();
	initializeModelTransform_ = animationParentMatrix_;
	initializeModelTranslation_ = initializeModelTransform_.GetTranslate(initializeModelTransform_);
}

Matrix MCB::Bone::GetAnimationMatrix()
{
	return animationParentMatrix_;
}

Quaternion MCB::Bone::GetRotation()
{
	return rotation_;
}

Vector3 MCB::Bone::GetModelTranslate()
{
	return modelTranslate_;
}

Bone* MCB::Bone::GetParent()
{
	return parent_;
}
std::vector<Bone*> MCB::Bone::GetChildren()
{
	return children_;
}

Vector3 MCB::Bone::GetTopLimitEulerRadian()
{
	return topLimitEulerRadian_;
}

Vector3 MCB::Bone::GetBottomLimitEulerRadian()
{
	return bottomLimitEulerRadian_;
}

Vector3 MCB::Bone::GetInitializeModelTranslate()
{
	return initializeModelTranslation_;
}

Quaternion MCB::Bone::GetInitializeRotation()
{
	return initializeRotation_;
}

void MCB::Bone::UpdateMatrix()
{
	Matrix scaleMat = Matrix::MatrixScaling(scale_);
	Matrix rotationMat = Matrix::GetQuaternionRotaMat(rotation_);
	Matrix translationMat = Matrix::MatrixTranslate(translation_);

	localTranform_ = scaleMat * rotationMat * translationMat;
	if (parent_)
	{
		animationParentMatrix_ = localTranform_ * parent_->GetAnimationMatrix();
	}
	else
	{
		animationParentMatrix_ = localTranform_;
	}

	modelTranslate_ = animationParentMatrix_.GetTranslate(animationParentMatrix_);
}
