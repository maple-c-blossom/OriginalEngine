#include "ImguiManager.h"
WarningIgnoreBegin
#include <MyImgui/imgui.h>
#include <MyImgui/imgui_impl_win32.h>
#include <MyImgui/imgui_impl_dx12.h>
WarningIgnoreEnd
#include "DxWindow.h"
#include "Dx12.h"
#include "Descriptor.h"
using namespace MCB;
using namespace ImGui;
void ImguiManager::Init()
{
	//D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	//desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	// 
	//desc.NumDescriptors = 1;
	//desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	//HRESULT result = S_OK;
	//result = Dx12::GetInstance()->device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvHeap));
	
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(DxWindow::GetInstance()->hwnd_);
	ImGui_ImplDX12_Init(Dx12::GetInstance()->device_.Get(), static_cast<int32_t>(Dx12::GetInstance()->backBuffers_.size()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,ShaderResource::GetInstance()->srvHeap_.Get(),
		ShaderResource::GetInstance()->srvHeap_->GetCPUDescriptorHandleForHeapStart(),
		ShaderResource::GetInstance()->srvHeap_->GetGPUDescriptorHandleForHeapStart());
	ImGuiIO& io = GetIO();
	io.Fonts->AddFontFromFileTTF("Resources\\meiryo.ttc",18.0f,NULL,io.Fonts->GetGlyphRangesJapanese());
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
	ID3D12GraphicsCommandList* commandList = Dx12::GetInstance()->commandList_.Get();
	ID3D12DescriptorHeap* ppHeaps[] = { ShaderResource::GetInstance()->srvHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
}


void MCB::ImguiManager::Final()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
