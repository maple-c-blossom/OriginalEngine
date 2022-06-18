#pragma once
#include "MCBFloat.h"
#include <d3d12.h>
#include <wrl.h>
#include "Dx12.h"
#include "DxWindow.h"
#include "Object3d.h"
#include "PIpelineRootSignature.h"
#include "Descriptor.h"
#include "Texture.h"
namespace MCB
{
	class Sprite
	{
	public:
		//定数バッファ用構造体(行列)------------------------
		typedef struct SpriteConstBufferDataTransform
		{
			Float4 color;
			DirectX::XMMATRIX mat;
		};
		//---------------------------------


		typedef struct SpriteVertex
		{
			Float3 pos;
			Float2 uv;
		};

		Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;
		D3D12_VERTEX_BUFFER_VIEW vbView{};
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuff = nullptr;
		D3D12_HEAP_PROPERTIES HeapProp{};
		D3D12_HEAP_PROPERTIES constHeapProp{};
		D3D12_RESOURCE_DESC Resdesc{};
		D3D12_RESOURCE_DESC constResdesc{};
		//Texture texture;


		Sprite CreateSprite(Dx12& dx12, DxWindow& dxWindow);
		void SpriteCommonBeginDraw(Dx12& dx12, const PipelineRootSignature& pipeline, ShaderResource& descHeap);
		void SpriteDraw(const Sprite& sprite,Dx12& dx12, ShaderResource descriptor, Texture& tex);
	};

}

