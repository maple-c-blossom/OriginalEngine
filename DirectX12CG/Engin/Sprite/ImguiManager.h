#pragma once
#include "IgnoreWarning.h"
WarningIgnoreBegin
#include <d3d12.h>
#include <wrl.h>
WarningIgnoreEnd
namespace MCB
{
	class ImguiManager
	{
		//Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;

		//D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;
	public:
		void Init();
		void Begin();
		void End();
		void Draw();
		void Final();
	};


}

