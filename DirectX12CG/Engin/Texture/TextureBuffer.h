#pragma once

#include <d3d12.h>
#include <wrl.h>
#include "TextureFile.h"
#include "Dx12.h"
#include "TexImgData.h"

namespace MCB
{

    class TextureBuffer
    {
    public:
        D3D12_HEAP_PROPERTIES texHeapProp_{};

        D3D12_RESOURCE_DESC texresDesc_{};

        Microsoft::WRL::ComPtr<ID3D12Resource> texbuff_ = nullptr;

        HRESULT CommitResouce(const D3D12_HEAP_FLAGS& flags, const D3D12_RESOURCE_STATES& resouceState,
            const D3D12_CLEAR_VALUE* clearValue);

        void TransferMipmatToTexBuff(const TextureFile &texFile, D3D12_BOX* DsrBox,HRESULT &result);
        
        void SetTexHeapProp(const D3D12_HEAP_TYPE& heaptype,const D3D12_CPU_PAGE_PROPERTY& cpuPagePropety,
            const D3D12_MEMORY_POOL& memorypool);

        void SetTexResourceDesc(TextureFile& texFile,const D3D12_RESOURCE_DIMENSION& resouceDimension,
            int32_t SampleDescCount);

        void SetNoTextureFileTexResourceDesc();

        void SetNoTextureFileTexResourceDescForPostEffect();

        void TransferMipmatToTexBuff(const TexImgData& teximg,HRESULT& result);
        void TransferMipmatToTexBuff(HRESULT& result);
    };
}

