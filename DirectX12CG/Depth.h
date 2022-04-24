#pragma once
#include <d3dcompiler.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include "DxWindow.h"
#include "Dx12.h"
#include <wrl.h>

#pragma region using namespace

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

#pragma endregion using namespace
class Depth
{
public:
	D3D12_RESOURCE_DESC depthResDesc{};
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	D3D12_CLEAR_VALUE depthClearValue{};
	ComPtr<ID3D12Resource> depthBuffer = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};

	Depth(DxWindow dxWindow, Dx12& dx12);


	void InitDepthResDesc(DxWindow dxWindow);

	void InitDepthHeapProp();

	void InitDepthClearValue();

	void InitDepthBuffer(Dx12 &dx12);

	void InitDepthDescriptorHeap(Dx12 &dx12);

	void InitDepthStencilView(Dx12& dx12);

	void SetDepthStencilState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc);
};

