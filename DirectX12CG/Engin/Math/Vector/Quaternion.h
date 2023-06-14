#pragma once
#include "Vector3D.h"
#include "MCBMatrix.h"
namespace MCB
{
	class Quaternion
	{
	public:
		float x_;
		float y_;
		float z_;
		float w_;
		Quaternion(const Vector3D& vec,  float angle);
		Quaternion();
		Quaternion( float x,  float y, float z, float w);

		//�^����ꂽ�x�N�g������̉�]��\���N�H�[�^�j�I���𐶐�
		void SetRota( Vector3D vec, float angle);
		//����
		Quaternion GetConjugated ( Quaternion q);
		//�t���i���w�p����
		Quaternion GetReciprocal( Quaternion q);
		//����
		double GetNorm();
		//q*p�̒���
		Quaternion GetDirectProduct(const Quaternion& q, const Quaternion& p);
		//����
		float Dot(const Quaternion& a, const Quaternion& b);
		//��̃N�H�[�^�j�I���̊p�x��
		float GetAngle(const Quaternion& a, const Quaternion& b, float& dot,bool& nan);
		float GetAngle(const Quaternion& a, const Quaternion& b);
		//���ʐ��`���
		Quaternion Slerp( Quaternion start, const Quaternion& end,  int32_t time,  int32_t maxTime);
		//���ʐ��`���
		Quaternion Slerp( Quaternion start, const Quaternion& end,  float time);
		//�^����ꂽ�x�N�g��������̉�]��PositionVec�ɗ^����
		Vector3D SetRotationVector(const Vector3D& rotationAxisVec,  Vector3D PositionVec,  float angle);
		//�^����ꂽ�N�H�[�^�j�I���̉�]��PositionVec�ɗ^����
		Vector3D SetRotationVector(const Quaternion& rotationQuaternion,  Vector3D PositionVec);
		//���̂��߂ɍ�����̍������ł��傤����
		Vector3D SetRotationVector(const Quaternion& rotationQuaternion, const Quaternion& PositionVec);

		Quaternion DirToDir( Vector3D u,  Vector3D v);
		//���K������
		void Normalize();
		//���K�������l��Ԃ�
		static Quaternion Normalize( Quaternion q);
		//��]�s��ɕϊ�
		MCBMatrix GetQuaternionRotaMat(const Quaternion& rotaQ);
		
		void SinCos(float* returnSin, float* returnCos, float theta);
		//�����I�C���[�p����N�H�[�^�j�I���𐶐�
		Quaternion SetToRorateObjectToInternal(const Float3& eulerAngle);
		//�N�H�[�^�j�I���̉�]�����Z�o
		Vector3D GetRotationAxis(const Quaternion& q);
		//�N�H�[�^�j�I���̉�]�����Z�o(AxisVec������������)
		void GetRotationAxis(const Quaternion& q,Vector3D& AxisVec);
		//�P�ʃN�H�[�^�j�I��
		Quaternion Identity();

		//�N�H�[�^�j�I���̊p�x
		float GetAngle(const Quaternion& q);
		
		float SafeAcos( float a);

		bool operator== (const Quaternion& q);
		Quaternion operator-();
	};
	Quaternion SetRota(const Vector3D& vec,  float angle);
}

