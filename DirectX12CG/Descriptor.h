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
            //定数バッファ用のデスクリプタヒープ

            //設定構造体
            D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};

            //デスクリプタヒープの生成  
            Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;
            
            D3D12_CPU_DESCRIPTOR_HANDLE srvHandle; 

            //シェーダーリソースビュー設定
            D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

            //デスクリプタレンジの設定
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

