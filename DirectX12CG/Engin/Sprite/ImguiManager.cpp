#include "Object3D.h"
#include "ImguiManager.h"
WarningIgnoreBegin
#include <MyImgui/imgui.h>
#include <MyImgui/imgui_impl_win32.h>
#include <MyImgui/imgui_impl_dx12.h>
WarningIgnoreEnd
#include "DxWindow.h"
#include "Dx12.h"
#include "Descriptor.h"
#include "ICamera.h"
#include "BaseCollider.h"
#include "CollisionManager.h"
#include "AnimationModel.h"

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

void MCB::ImguiManager::GuizmoEnable(bool flag)
{
	ImGuizmo::Enable(flag);
}

void MCB::ImguiManager::Begin()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
	ImGuizmo::SetRect(0.f,0.f,static_cast<float>(DxWindow::GetInstance()->sWINDOW_WIDTH_),
		static_cast< float >(DxWindow::GetInstance()->sWINDOW_CENTER_WIDTH_));
}

void MCB::ImguiManager::End()
{
	ImGui::Render();
}

 void MCB::ImguiManager::GuizmoDraw(MCB::Object3d* obj,ImGuizmo::OPERATION operation,
						ImGuizmo::MODE mode, Float3 snap,float* deltaMat)
{
	MCBMatrix tempMatV(obj->camera_->GetView()->mat_);
	MCBMatrix tempMatP(obj->camera_->GetProjection()->mat_);
	MCBMatrix tempMatW(obj->matWorld_.matWorld_);
	float* v = tempMatV.matOneArray;
	float* p = tempMatP.matOneArray;
	float* w = tempMatW.matOneArray;
	ImGuizmo::Manipulate(v,p,operation,mode,w,deltaMat,snap.arrayFloat);
	tempMatW = w;
	obj->matWorld_.matWorld_ = tempMatW.MatrixConvertXMMatrix(tempMatW);
	DirectX::XMVECTOR scaleVec = DirectX::XMLoadFloat3(&obj->scale_);
	DirectX::XMVECTOR rotateVec = DirectX::XMLoadFloat3(&obj->rotation_);
	DirectX::XMVECTOR positionVec = DirectX::XMLoadFloat3(&obj->position_);
	DirectX::XMMatrixDecompose(&scaleVec,&rotateVec,&positionVec,obj->matWorld_.matWorld_);
	obj->scale_.x = scaleVec.m128_f32[0]; obj->scale_.y = scaleVec.m128_f32[1]; obj->scale_.z = scaleVec.m128_f32[2];
	obj->rotation_.x = rotateVec.m128_f32[0]; obj->rotation_.y = rotateVec.m128_f32[1]; obj->rotation_.z = rotateVec.m128_f32[2];
	obj->position_.x = positionVec.m128_f32[0]; obj->position_.y = positionVec.m128_f32[1]; obj->position_.z = positionVec.m128_f32[2];
	obj->constMapTranceform_->world = obj->matWorld_.matWorld_;
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
