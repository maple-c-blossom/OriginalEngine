#pragma once
#include <string>
#include <vector>
#include "Vector3.h"
#include "Quaternion.h"
#include "Matrix.h"

namespace MCB
{
	class Bone
	{
	private:
		std::string name_;
		int32_t num;

		Vector3 scale_;
		Vector3 translation_;
		Quaternion rotation_;
		Bone* parent_;
		std::vector<Bone*> children_;
		
		Matrix localTranform_;
		Matrix modelInverseTransform_;
		Matrix animationParentMatrix_;
		Matrix offsetMatrix_;
		Matrix finalMatrix_;

		Vector3 initializeScale_;
		Quaternion initializeRotation_;
		Vector3 initializeLocalTranslation_;
		Matrix initializeModelTransform_;
		Vector3 initializeModelTranslation_;

		Vector3 modelTranslate_;

		Vector3 topLimitEulerRadian_;
		Vector3 bottomLimitEulerRadian_;
	public:
		void SetRotation(const Quaternion& rot);
		void SetName(const std::string& name);
		void SetScale(const Vector3& scale);
		void SetTranslation(const Vector3& translation);
		void SetModelTranslation(const Vector3& translation);
		void SetParent(Bone* parent);
		void SetBoneRotationLimit(Vector3 topEulerLimit, Vector3 bottomEulerLimit);
		std::string GetName();
		Vector3 GetScale();
		Vector3 GetTranslation();
		Matrix GetOffSetMatrix();
		Matrix GetFinalMatrix();
		void AddChild(Bone* child);
		void SetFinalMatrix(const Matrix& matrix);
		void SetOffsetMatrix(const Matrix& matrix);
		void RemoveChild(Bone* child);
		//親のボーンを設定し、設定した親のボーンの子に自身を追加する。
		//また既に親のボーンが設定されていた場合、その親のボーンの子ボーンから自身を削除する。
		void SetParentAndChild(Bone* parent);

		//現在のデータを初期値として設定
		void SetInitializeTransformData();


		Matrix GetAnimationMatrix();
		Quaternion GetRotation();
		Vector3 GetModelTranslate();
		Bone* GetParent();
		std::vector<Bone*> GetChildren();
		Vector3 GetTopLimitEulerRadian();
		Vector3 GetBottomLimitEulerRadian();
		Vector3 GetInitializeModelTranslate();
		Quaternion GetInitializeRotation();

		void UpdateMatrix();
	};
}

