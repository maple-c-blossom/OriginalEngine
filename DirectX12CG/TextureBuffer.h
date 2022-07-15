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
        D3D12_HEAP_PROPERTIES texHeapProp{};

        D3D12_RESOURCE_DESC texresDesc{};

        Microsoft::WRL::ComPtr<ID3D12Resource> texbuff = nullptr;

        HRESULT CommitResouce(D3D12_HEAP_FLAGS flags, D3D12_RESOURCE_STATES resouceState, const D3D12_CLEAR_VALUE* clearValue);

        void TransferMipmatToTexBuff(const TextureFile &texFile, D3D12_BOX* DsrBox,HRESULT &result);
        
        void SetTexHeapProp(D3D12_HEAP_TYPE heaptype, D3D12_CPU_PAGE_PROPERTY cpuPagePropety, D3D12_MEMORY_POOL memorypool);

        void SetTexResourceDesc(TextureFile& texFile, D3D12_RESOURCE_DIMENSION resouceDimension, int SampleDescCount);

        void SetNoTextureFileTexResourceDesc();

        void TransferMipmatToTexBuff(TexImgData teximg,HRESULT& result);
    };
}

