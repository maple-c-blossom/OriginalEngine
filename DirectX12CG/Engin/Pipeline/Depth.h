#pragma once
#include <d3dcompiler.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include "DxWindow.h"
#include "Dx12.h"
#include <wrl.h>

namespace MCB
{
	class Depth
	{
	public:
		D3D12_RESOURCE_DESC depthResDesc_{};
		D3D12_HEAP_PROPERTIES depthHeapProp_{};
		D3D12_CLEAR_VALUE depthClearValue_{};
		Microsoft::WRL::ComPtr<ID3D12Resource> depthBuffer_ = nullptr;
		D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc_{};
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap_ = nullptr;
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc_ = {};

		Depth();


		void InitDepthResDesc();

		void InitDepthHeapProp();

		void InitDepthClearValue();

		void InitDepthBuffer();

		void InitDepthDescriptorHeap();

		void InitDepthStencilView();

		void SetDepthStencilState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc);
		void SetSpriteDepthStencilState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc);
		void SetParticleDepthStencilState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc);
	};
}

