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
#include "Projection.h"
#pragma region using namespace

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

#pragma endregion using namespace
class ObjectMaterial
{
public:
	//定数バッファ用構造体(マテリアル)-----------------------------------
	typedef struct ConstBufferDataMaterial
	{
		XMFLOAT4 color;
	};
	//------------------------------------------
	D3D12_HEAP_PROPERTIES HeapProp{};
	D3D12_RESOURCE_DESC Resdesc{};
	ComPtr<ID3D12Resource> constBuffMaterial = nullptr;
	ConstBufferDataMaterial* constMapMaterial = nullptr;

	void Init(Dx12 dx12);

};

