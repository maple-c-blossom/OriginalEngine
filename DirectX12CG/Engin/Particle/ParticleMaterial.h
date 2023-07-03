#pragma once
#include <d3dcompiler.h>
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <wrl.h>
#include "WorldMatrix.h"
#include "Dx12.h"
#include "View.h"
#include "Vertex.h"
#include "MCBFloat.h"

namespace MCB
{
	class ParticleMaterial
	{
	public:
		////定数バッファ用構造体(マテリアル)-----------------------------------
		//typedef struct ConstBufferDataMaterial
		//{
		//	DirectX::XMFLOAT4 color = { 0,0,0,1 };
		//};
		////------------------------------------------

		typedef struct ConstBufferDataMaterialB1
		{

			Float3 ambient;
			float pad1;
			Float3 diffuse;
			float pad2;
			Float3 specular;
			float alpha;
			Float4 color;
		}ConstBufferDataMaterialB1;


		typedef struct Material
		{
			std::string name;
			Float3 ambient;
			Float3 diffuse;
			Float3 specular;
			float alpha;
			Float4 color;
			std::string textureFileName;

			Material()
			{
				ambient = { 0.3f,0.3f,0.3f };
				diffuse = { 0.0f,0.0f,0.0f };
				specular = { 0.0f,0.0f,0.0f };
				alpha = 1.0f;
				color = { 1.f,1.f,1.f,1.f };
			}
		}Material;



		Material material_;
		D3D12_HEAP_PROPERTIES HeapProp_{};
		D3D12_RESOURCE_DESC Resdesc_{};
		//Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterialB1_ = nullptr;
		ConstBufferDataMaterialB1* constMapMaterial_ = nullptr;

		~ParticleMaterial();

		void Init();

		void SetIndex(const D3D12_RESOURCE_DIMENSION& dimension, uint32_t sizeIB,
			 int32_t height, int32_t DepthOrArraySize,  int32_t MipLevels,
			 int32_t SampleDescCount, const D3D12_TEXTURE_LAYOUT& layout);

		void SetVertexBuffer(const D3D12_HEAP_TYPE& heaptype,const D3D12_RESOURCE_DIMENSION& dimension,
			 uint32_t sizeVB,  int32_t height,  int32_t DepthOrArraySize, 
			 int32_t MipLevels,  int32_t SampleDescCount, const D3D12_TEXTURE_LAYOUT& layout);

	};

}

