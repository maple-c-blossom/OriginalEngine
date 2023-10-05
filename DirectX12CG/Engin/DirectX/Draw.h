#pragma once
#include "Dx12.h"
#include "Depth.h"
#include "Descriptor.h"
#include "PIpelineRootSignature.h"
#include "DxWindow.h"
#include "Pading.h"
#include "IgnoreWarning.h"
WarningIgnoreBegin
#include <memory>
WarningIgnoreEnd

namespace MCB
{
	class Draw
	{
	private:
		Draw();
		Draw(const Draw&);
		Draw& operator=(const Draw&);
		~Draw();
	public:
		D3D12_RESOURCE_BARRIER barrierDesc_ = {};
		uint32_t bbIndex_ = {};
		Byte4 pad;
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle_ = {};
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle_ = {};
		D3D12_VIEWPORT viewport_ = {};
		D3D12_RECT scissorrect_ = {};


		D3D12_RESOURCE_BARRIER GetResouceBarrier();
		uint32_t GetBbIndex();

		static Draw* GetInstance();



		void SetBeforeBbIndex();
		void SetBeforeResourceBarrier();
		void SetBeforeResourceBarrierAfterPostEffect();
		void BeforeDraw(const Depth& depth, const PipelineRootSignature& pipeline);
		void SetRenderTargetView(const Depth& depth);
		void ClearScreen(const float* clearColor);
		void SetViewPort();
		void SetScissorrect();
		void PreDraw(const Depth& depth,const PipelineRootSignature& pipeline, const float* clearColor);
		void AfterPostEffectPreDraw(const Depth& depth,const PipelineRootSignature& pipeline, const float* clearColor);


		void SetAfterResourceBarrier();
		void CloseDrawCommandOrder();
		void CommandListExecution();
		void ResetQueAndCommandList();
		void PostDraw();
	};

}

