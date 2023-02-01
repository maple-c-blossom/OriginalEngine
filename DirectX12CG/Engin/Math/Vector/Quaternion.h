#pragma once
#include "Vector3D.h"
#include "MCBMatrix.h"
namespace MCB
{
	class Quaternion
	{
	public:
		float x;
		float y;
		float z;
		float w;
		Quaternion(Vector3D vec, float angle);
		Quaternion();
		Quaternion(float x, float y,float z,float w);

		//�^����ꂽ�x�N�g������̉�]��\���N�H�[�^�j�I���𐶐�
		void SetRota(Vector3D vec,float angle);
		//����
		Quaternion GetConjugated(Quaternion q);
		//�t���i���w�p����j
		Quaternion GetReciprocal(Quaternion q);
		//����
		double GetNorm();
		//q*p�̒���
		Quaternion GetDirectProduct(Quaternion q, Quaternion p);
		//����
		float Dot(Quaternion a, Quaternion b);
		//��̃N�H�[�^�j�I���̊p�x��
		float GetAngle(Quaternion a, Quaternion b, float& dot,bool& nan);
		float GetAngle(Quaternion a, Quaternion b);
		//���ʐ��`���
		Quaternion Slerp(Quaternion start, Quaternion end, int time,int maxTime);
		//���ʐ��`���
		Quaternion Slerp(Quaternion start, Quaternion end, float time);
		//�^����ꂽ�x�N�g��������̉�]��PositionVec�ɗ^����
		Vector3D SetRotationVector(Vector3D rotationAxisVec, Vector3D PositionVec, float angle);
		//�^����ꂽ�N�H�[�^�j�I���̉�]��PositionVec�ɗ^����
		Vector3D SetRotationVector(Quaternion rotationQuaternion, Vector3D PositionVec);
		//���̂��߂ɍ�����̍������ł��傤����
		Vector3D SetRotationVector(Quaternion rotationQuaternion, Quaternion PositionVec);

		Quaternion DirToDir(Vector3D u, Vector3D v);
		//���K������
		void Normalize();
		//���K�������l��Ԃ�
		static Quaternion Normalize(Quaternion q);
		//��]�s��ɕϊ�
		MCBMatrix GetQuaternionRotaMat(Quaternion rotaQ);
		
		void SinCos(float* returnSin, float* returnCos, float theta);
		//�����I�C���[�p����N�H�[�^�j�I���𐶐�
		Quaternion SetToRorateObjectToInternal(const Float3 eulerAngle);
		//�N�H�[�^�j�I���̉�]�����Z�o
		Vector3D GetRotationAxis(Quaternion q);
		//�N�H�[�^�j�I���̉�]�����Z�o(AxisVec������������)
		void GetRotationAxis(Quaternion q,Vector3D& AxisVec);
		//�P�ʃN�H�[�^�j�I��
		Quaternion Identity();

		//�N�H�[�^�j�I���̊p�x
		float GetAngle(Quaternion q);
		
		float SafeAcos(float a);

		bool operator== (Quaternion q);
		Quaternion operator-();
	};
	Quaternion SetRota(Vector3D vec, float angle);
}

