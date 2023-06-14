#pragma once

#include <d3d12.h>
#include <wrl.h>
#include "Dx12.h"
#include "TextureBuffer.h"
#include <array>

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
            D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc_{};

            //デスクリプタヒープの生成  
            Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_ = nullptr;
            
            D3D12_CPU_DESCRIPTOR_HANDLE srvHandle_; 

            //シェーダーリソースビュー設定
            D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc_{};

            //デスクリプタレンジの設定
            std::array<D3D12_DESCRIPTOR_RANGE, 2> descriptorRange_ = {};

            static uint16_t sAllincrementNum_;

            const size_t MAX_SRV_COUNT_ = 0xfff;


            static ShaderResource* GetInstance();

            static ShaderResource* GetInitInstance();


            void Init();

            void SetHeapDesc(const D3D12_DESCRIPTOR_HEAP_FLAGS& flags);

            HRESULT SetDescriptorHeap();

            void SetSrvHeap();

            void SetSrvHeap( uint16_t incrementNum);

            void InitAllincrementNum();

            void SetSrvDesc(TextureBuffer &texBuffer, const D3D12_SRV_DIMENSION& srvDimension);

            void SetShaderResourceView(TextureBuffer& texBuffer);

            void SetDescriptorRange ( int32_t NumDescriptors, const D3D12_DESCRIPTOR_RANGE_TYPE& type,
                int32_t BaseShaderRegister,  size_t index = 0);
	};
}

