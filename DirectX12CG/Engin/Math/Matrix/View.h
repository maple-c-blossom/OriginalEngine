#pragma once
#include <DirectXMath.h>


namespace MCB
{
    class View
    {
        public:
        DirectX::XMMATRIX mat_ = DirectX::XMMatrixIdentity();//�s��
        DirectX::XMFLOAT3 eye_{ 0,0,0 }; //���_���W(�J�������g�̍��W)
        //�����_���W(�J���������Ă�����W�AXMMatrixLookAtLH�͂��̍��W����
        //���_���W�ւ̃x�N�g�������߂邱�ƂŌ��������肵�Ă���)
        DirectX::XMFLOAT3 target_{ 0,0,1 };
        DirectX::XMFLOAT3 up_{ 0,1,0 };//������x�N�g��(�J�����̏㑤���ǂ��������肷����W�B)
        DirectX::XMMATRIX billMat_ = DirectX::XMMatrixIdentity();


        /// <summary>
        /// �����̒l���N���X�̂��ꂼ��̕ϐ��ɑ��������Ńr���[�ϊ��s����쐬���܂��B
        /// </summary>
        /// <param name="eye">���_���W</param>
        /// <param name="target">�����_���W</param>
        /// <param name="up">������x�N�g��</param>
        void CreateMatrixView(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up);

        /// <summary>
        /// ���łɂ���ϐ��𗘗p���čs����쐬���܂�
        /// </summary>
        void UpDateMatrixView( bool billbordY = false);
    };

}

