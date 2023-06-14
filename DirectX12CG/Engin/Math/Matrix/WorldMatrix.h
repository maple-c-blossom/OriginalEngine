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
		void SetMatRotX( float angle,  bool ConvertToRadians = true);
		/// <summary>
		/// Y��]�ݒ�
		/// </summary>
		/// <param name="angle">�p�x</param>
		/// <param name="ConvertToRadians">�ʓx�@�ɕϊ�����t���O�Aangle���ʓx�@�ŏ�������"false"��ݒ肵�Ă�������</param>
		void SetMatRotY( float angle,  bool ConvertToRadians = true);
		/// <summary>
		/// Z��]�̐ݒ�
		/// </summary>
		/// <param name="angle">�p�x</param>
		/// <param name="ConvertToRadians">�ʓx�@�ɕϊ�����t���O�Aangle���ʓx�@�ŏ�������"false"��ݒ肵�Ă�������</param>
		void SetMatRotZ( float angle,  bool ConvertToRadians = true);

		/// <summary>
		/// ��]�ݒ�
		/// </summary>
		/// <param name="angleX">X�̊p�x</param>
		/// <param name="angleY">Y�̊p�x</param>
		/// <param name="angleZ">Z�̊p�x</param>
		/// <param name="ConvertToRadians">�ʓx�@�ɕϊ�����t���O�Aangle���ʓx�@�ŏ�������"false"��ݒ肵�Ă�������</param>
		void SetMatRot( float angleX,  float angleY,  float angleZ,  bool ConvertToRadians = true);



		/// <summary>
		/// �����ŗ^����ꂽ�p�x�ɕϊ����ꂽ��]�s���Ԃ��܂�
		/// </summary>
		/// <param name="matRot">��]���̍s��</param>
		/// <param name="angleX">X�̊p�x</param>
		/// <param name="angleY">Y�̊p�x</param>
		/// <param name="angleZ">Z�̊p�x</param>
		/// <param name="ConvertToRadians">�ʓx�@�ɕϊ�����t���O�Aangle���ʓx�@�ŏ�������"false"��ݒ肵�Ă�������</param>
		/// <returns></returns>
		DirectX::XMMATRIX ReturnMatRot(const DirectX::XMMATRIX& matRot,  float angleX,  float angleY,
			 float angleZ,  bool ConvertToRadians = true);

		void SetMatScale( float scaleX,  float scaleY,  float scaleZ);

		void SetMatTrans( float transX,  float transY,  float transZ);

		void CreateMatrixWorld(const DirectX::XMMATRIX& matScale, const DirectX::XMMATRIX& matRot, 
			const DirectX:: XMMATRIX& matTransform);

		void UpdataBillBordMatrixWorld(const View& view);

		void UpdataMatrixWorld();


	};
}

