#pragma once

#include <d3d12.h>
#include <wrl.h>
#include "Dx12.h"
#include "TextureBuffer.h"

namespace MCB
{

	class ShaderResource
	{
        ShaderResource() {};
        ShaderResource(const ShaderResource& shaderResource) {};
        ShaderResource& operator=(const ShaderResource& shaderResource) {};
        ~ShaderResource() {};
		public:
            //�萔�o�b�t�@�p�̃f�X�N���v�^�q�[�v

            //�ݒ�\����
            D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};

            //�f�X�N���v�^�q�[�v�̐���  
            Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;
            
            D3D12_CPU_DESCRIPTOR_HANDLE srvHandle; 

            //�V�F�[�_�[���\�[�X�r���[�ݒ�
            D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

            //�f�X�N���v�^�����W�̐ݒ�
            D3D12_DESCRIPTOR_RANGE descriptorRange{};

            static unsigned short int AllincrementNum;

            const size_t MaxSRVCount = 0xfff;


            static ShaderResource* GetInstance();
            static void DeleteInstace();
            static ShaderResource* GetInitInstance();


            void Init();

            void SetHeapDesc(D3D12_DESCRIPTOR_HEAP_FLAGS flags);

            HRESULT SetDescriptorHeap();

            void SetSrvHeap();

            void SetSrvHeap(unsigned short int incrementNum);

            void InitAllincrementNum();

            void SetSrvDesc(TextureBuffer &texBuffer, D3D12_SRV_DIMENSION srvDimension);

            void SetShaderResourceView(TextureBuffer& texBuffer);

            void SetDescriptorRange(int NumDescriptors, D3D12_DESCRIPTOR_RANGE_TYPE type,int BaseShaderRegister);
	};
}

