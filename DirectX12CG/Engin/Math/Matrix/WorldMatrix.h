#pragma once

#include <DirectXMath.h>
#include "View.h"

namespace MCB
{
	class WorldMatrix
	{
	public:
		//���[���h�s��
		DirectX::XMMATRIX matWorld = DirectX::XMMatrixIdentity();
		//�X�P�[���s��
		DirectX::XMMATRIX matScale = DirectX::XMMatrixIdentity();
		//��]�s��
		DirectX::XMMATRIX matRot = DirectX::XMMatrixIdentity();
		//���s�ړ��s��
		DirectX::XMMATRIX matTransform;





		/// <summary>
		/// X��]�̐ݒ�
		/// </summary>
		/// <param name="angle">�p�x</param>
		/// <param name="ConvertToRadians">�ʓx�@�ɕϊ�����t���O�Aangle���ʓx�@�ŏ�������"false"��ݒ肵�Ă�������</param>
		void SetMatRotX(float angle,bool ConvertToRadians = true);
		/// <summary>
		/// Y��]�ݒ�
		/// </summary>
		/// <param name="angle">�p�x</param>
		/// <param name="ConvertToRadians">�ʓx�@�ɕϊ�����t���O�Aangle���ʓx�@�ŏ�������"false"��ݒ肵�Ă�������</param>
		void SetMatRotY(float angle, bool ConvertToRadians = true);
		/// <summary>
		/// Z��]�̐ݒ�
		/// </summary>
		/// <param name="angle">�p�x</param>
		/// <param name="ConvertToRadians">�ʓx�@�ɕϊ�����t���O�Aangle���ʓx�@�ŏ�������"false"��ݒ肵�Ă�������</param>
		void SetMatRotZ(float angle, bool ConvertToRadians = true);

		/// <summary>
		/// ��]�ݒ�
		/// </summary>
		/// <param name="angleX">X�̊p�x</param>
		/// <param name="angleY">Y�̊p�x</param>
		/// <param name="angleZ">Z�̊p�x</param>
		/// <param name="ConvertToRadians">�ʓx�@�ɕϊ�����t���O�Aangle���ʓx�@�ŏ�������"false"��ݒ肵�Ă�������</param>
		void SetMatRot(float angleX, float angleY, float angleZ, bool ConvertToRadians = true);

		/// <summary>
		/// �����ŗ^����ꂽ�p�x�ɕϊ����ꂽ��]�s���Ԃ��܂�
		/// </summary>
		/// <param name="matRot">��]���̍s��</param>
		/// <param name="angleX">X�̊p�x</param>
		/// <param name="angleY">Y�̊p�x</param>
		/// <param name="angleZ">Z�̊p�x</param>
		/// <param name="ConvertToRadians">�ʓx�@�ɕϊ�����t���O�Aangle���ʓx�@�ŏ�������"false"��ݒ肵�Ă�������</param>
		/// <returns></returns>
		DirectX::XMMATRIX ReturnMatRot(DirectX::XMMATRIX matRot,float angleX, float angleY, float angleZ, bool ConvertToRadians = true);

		void SetMatScale(float scaleX, float scaleY, float scaleZ);

		void SetMatTrans(float transX, float transY, float transZ);

		void CreateMatrixWorld(DirectX::XMMATRIX matScale, DirectX::XMMATRIX matRot,DirectX:: XMMATRIX matTransform);

		void UpdataBillBordMatrixWorld(View view);

		void UpdataMatrixWorld();


	};
}

