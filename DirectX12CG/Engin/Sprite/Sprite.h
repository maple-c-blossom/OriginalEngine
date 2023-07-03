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
		}SpriteConstBufferDataTransform;
		//---------------------------------

		typedef struct SpriteVertex
		{
			Float3 pos;
			Float2 uv;
		}SpriteVertex;
		Sprite();

		Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_ = nullptr;
		D3D12_VERTEX_BUFFER_VIEW vbView_{};
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_ = nullptr;
		D3D12_HEAP_PROPERTIES HeapProp_{};
		D3D12_HEAP_PROPERTIES constHeapProp_{};
		D3D12_RESOURCE_DESC Resdesc_{};
		D3D12_RESOURCE_DESC constResdesc_{};

		float rotation_ = 0.0f;
		Float3 position_ = { 0,0,0 };
		DirectX::XMMATRIX matWorld_;
		Float4 color_ = { 1.0f,1.0f,1.0f,1.0f };
		static DirectX::XMMATRIX smatProje_;
		Float2 size_ = { 100,100 };
		Float2 anchorPoint_ = { 0.5f,0.5f };
		Float2 texLeftTop_ = { 0,0 };
		Float2 cuttingSize_ = { 0,0 };
		bool isFlipX_ = false;
		bool isFlipY_ = false;


		void SpriteTransferVertexBuffer( Texture* tex = nullptr);
		void SpriteUpdate();
		void InitMatProje();
		void CreateSprite();
		//void SpriteCommonBeginDraw(const PipelineRootSignature& pipeline);

		void SpriteDraw(Texture& tex,  float positionX = 0,  float positionY = 0,
			 float size_x = 0,  float size_y = 0);

		void SpriteFlipDraw( Texture& tex,  float positionX = 0,  float positionY = 0,
			 bool isflipX = false,  bool isflipY = false);

		void SpriteCuttingDraw(Texture& tex,  float positionX = 0,  float positionY = 0,
			const Float2& cuttingsize = {0,0}, const Float2& CuttingLeftTop = {0,0});




	};

}

