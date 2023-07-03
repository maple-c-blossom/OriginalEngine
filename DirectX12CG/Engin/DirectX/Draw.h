#pragma once
#include "Dx12.h"
#include "Depth.h"
#include "Descriptor.h"
#include "PIpelineRootSignature.h"
#include "DxWindow.h"
#include <memory>

namespace MCB
{
	class Draw
	{
	private:
		Draw() {};
		Draw(const Draw& draw) {};
		Draw& operator=(const Draw& draw) {};
		~Draw() {};
	public:
		D3D12_RESOURCE_BARRIER barrierDesc_ = {};
		uint32_t bbIndex_ = {};
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle_ = {};
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle_ = {};
		D3D12_VIEWPORT viewport_ = {};
		D3D12_RECT scissorrect_ = {};


		D3D12_RESOURCE_BARRIER GetResouceBarrier();
		uint32_t GetBbIndex();

		static Draw* GetInstance();



		void SetBeforeBbIndex();
		void SetBeforeResourceBarrier();
		void BeforeDraw(const Depth& depth, const PipelineRootSignature& pipeline);
		void SetRenderTargetView(const Depth& depth);
		void ClearScreen(const float* clearColor);
		void SetViewPort();
		void SetScissorrect();
		void PreDraw(const Depth& depth,const PipelineRootSignature& pipeline, const float* clearColor);


		void SetAfterResourceBarrier();
		void CloseDrawCommandOrder();
		void CommandListExecution();
		void ResetQueAndCommandList();
		void PostDraw();
	};

}

