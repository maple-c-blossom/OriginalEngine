#pragma once

#include <DirectXMath.h>
#include "View.h"

namespace MCB
{
	class WorldMatrix
	{
	public:
		//���[���h�s��
		DirectX::XMMATRIX matWorld_ = DirectX::XMMatrixIdentity();
		//�X�P�[���s��
		DirectX::XMMATRIX matScale_ = DirectX::XMMatrixIdentity();
		//��]�s��
		DirectX::XMMATRIX matRot_ = DirectX::XMMatrixIdentity();
		//���s�ړ��s��
		DirectX::XMMATRIX matTransform_;





		/// <summary>
		/// X��]�̐ݒ�
		/// </summary>
		/// <param name="angle">�p�x</param>
		/// <param name="ConvertToRadians">�ʓx�@�ɕϊ�����t���O�Aangle���ʓx�@�ŏ�������"false"��ݒ肵�Ă�������</param>
		void SetMatRotX(const float& angle, const bool& ConvertToRadians = true);
		/// <summary>
		/// Y��]�ݒ�
		/// </summary>
		/// <param name="angle">�p�x</param>
		/// <param name="ConvertToRadians">�ʓx�@�ɕϊ�����t���O�Aangle���ʓx�@�ŏ�������"false"��ݒ肵�Ă�������</param>
		void SetMatRotY(const float& angle, const bool& ConvertToRadians = true);
		/// <summary>
		/// Z��]�̐ݒ�
		/// </summary>
		/// <param name="angle">�p�x</param>
		/// <param name="ConvertToRadians">�ʓx�@�ɕϊ�����t���O�Aangle���ʓx�@�ŏ�������"false"��ݒ肵�Ă�������</param>
		void SetMatRotZ(const float& angle, const bool& ConvertToRadians = true);

		/// <summary>
		/// ��]�ݒ�
		/// </summary>
		/// <param name="angleX">X�̊p�x</param>
		/// <param name="angleY">Y�̊p�x</param>
		/// <param name="angleZ">Z�̊p�x</param>
		/// <param name="ConvertToRadians">�ʓx�@�ɕϊ�����t���O�Aangle���ʓx�@�ŏ�������"false"��ݒ肵�Ă�������</param>
		void SetMatRot(const float& angleX, const float& angleY, const float& angleZ, const bool& ConvertToRadians = true);



		/// <summary>
		/// �����ŗ^����ꂽ�p�x�ɕϊ����ꂽ��]�s���Ԃ��܂�
		/// </summary>
		/// <param name="matRot">��]���̍s��</param>
		/// <param name="angleX">X�̊p�x</param>
		/// <param name="angleY">Y�̊p�x</param>
		/// <param name="angleZ">Z�̊p�x</param>
		/// <param name="ConvertToRadians">�ʓx�@�ɕϊ�����t���O�Aangle���ʓx�@�ŏ�������"false"��ݒ肵�Ă�������</param>
		/// <returns></returns>
		DirectX::XMMATRIX ReturnMatRot(const DirectX::XMMATRIX& matRot, const float& angleX, const float& angleY,
			const float& angleZ, const bool& ConvertToRadians = true);

		void SetMatScale(const float& scaleX, const float& scaleY, const float& scaleZ);

		void SetMatTrans(const float& transX, const float& transY, const float& transZ);

		void CreateMatrixWorld(const DirectX::XMMATRIX& matScale, const DirectX::XMMATRIX& matRot, 
			const DirectX:: XMMATRIX& matTransform);

		void UpdataBillBordMatrixWorld(const View& view);

		void UpdataMatrixWorld();


	};
}

