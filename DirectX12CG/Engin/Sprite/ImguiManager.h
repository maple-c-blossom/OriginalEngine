#pragma once
#include "IgnoreWarning.h"
WarningIgnoreBegin
#include <d3d12.h>
#include <wrl.h>
#include <MyImgui/ImGuizmo.h>
WarningIgnoreEnd
namespace MCB
{
	class Object3D;
	class ImguiManager
	{
		//Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;

		//D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;
	public:
		void Init();
		void GuizmoEnable(bool flag);
		void Begin();
		void End();
		void GuizmoDraw(Object3D* obj,ImGuizmo::OPERATION operation,ImGuizmo::MODE mode, Float3 snap = {0.f,0.f,0.f},float* deltaMat = nullptr);
		void Draw();
		void Final();
	};


}

