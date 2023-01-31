#pragma once
#include "Dx12.h"
#include "Depth.h"
#include "Descriptor.h"
#include "PIpelineRootSignature.h"
#include "DxWindow.h"

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
		D3D12_RESOURCE_BARRIER barrierDesc;
		UINT bbIndex;
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;
		D3D12_VIEWPORT viewport;
		D3D12_RECT scissorrect;


		D3D12_RESOURCE_BARRIER GetResouceBarrier();
		UINT GetBbIndex();

		static Draw* GetInstance();
		static void DeleteInstace();


		void SetBeforeBbIndex();
		void SetBeforeResourceBarrier();
		void BeforeDraw(Depth depth, PipelineRootSignature pipeline);
		void SetRenderTargetView(Depth& depth);
		void ClearScreen(const float* clearColor);
		void SetViewPort();
		void SetScissorrect();
		void PreDraw(Depth depth,PipelineRootSignature pipeline, const float* clearColor);


		void SetAfterResourceBarrier();
		void CloseDrawCommandOrder();
		void CommandListExecution();
		void ResetQueAndCommandList();
		void PostDraw();
	};

}

