#include "ImguiManager.h"
#include <MyImgui/imgui.h>
#include <MyImgui/imgui_impl_win32.h>
#include <MyImgui/imgui_impl_dx12.h>
#include "DxWindow.h"
#include "Dx12.h"
#include "Descriptor.h"
using namespace MCB;
using namespace ImGui;
void ImguiManager::Init()
{
	//D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	//desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	//desc.NumDescriptors = 1;
	//desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	//HRESULT result = S_OK;
	//result = Dx12::GetInstance()->device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvHeap));
	
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(DxWindow::GetInstance()->hwnd);
	ImGui_ImplDX12_Init(Dx12::GetInstance()->device.Get(), Dx12::GetInstance()->backBuffers.size(),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,ShaderResource::GetInstance()->srvHeap.Get(),
		ShaderResource::GetInstance()->srvHeap->GetCPUDescriptorHandleForHeapStart(),
		ShaderResource::GetInstance()->srvHeap->GetGPUDescriptorHandleForHeapStart());
	ImGuiIO& io = GetIO();
	io.Fonts->AddFontDefault();
}

void MCB::ImguiManager::Begin()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void MCB::ImguiManager::End()
{
	ImGui::Render();
}

void MCB::ImguiManager::Draw()
{
	ID3D12GraphicsCommandList* commandList = Dx12::GetInstance()->commandList.Get();
	ID3D12DescriptorHeap* ppHeaps[] = { ShaderResource::GetInstance()->srvHeap.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
}


void MCB::ImguiManager::Final()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
