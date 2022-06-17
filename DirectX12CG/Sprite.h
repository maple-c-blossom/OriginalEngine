#pragma once
#include "MCBFloat.h"
#include <d3d12.h>
#include <wrl.h>
#include "Dx12.h"
#include "DxWindow.h"

namespace MCB
{
	class Sprite
	{
		typedef struct SpriteVertex
		{
			Float3 pos;
			Float2 uv;
		};

		Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;
		D3D12_VERTEX_BUFFER_VIEW vbView{};
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuff = nullptr;
		D3D12_HEAP_PROPERTIES HeapProp{};
		D3D12_RESOURCE_DESC Resdesc{};

		Sprite CreateSprite(Dx12& dx12, DxWindow& dxWindow);
	};

}

