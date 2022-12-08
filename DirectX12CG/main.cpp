#define _CRTDBG_MAP_ALLOC 
#include <crtdbg.h>
#include <cstdlib>
#define _USE_MATH_DEFINES
#include <cmath>
#include "SceneManager.h"
#include "Scene.h"
#include "FPS.h"




using namespace MCB;
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    {
        DxWindow* dxWindow = DxWindow::GetInitInstance();
        FPS* fps = FPS::GetInitInstance();

    //�f�o�b�N���̂�----------
#pragma region �f�o�b�N���̂�
#ifdef _DEBUG
//�f�o�b�N���C���[���I����
    Microsoft::WRL::ComPtr<ID3D12Debug1> debugController;
    Microsoft::WRL::ComPtr<ID3D12InfoQueue> infoQueue;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
    {
        debugController->EnableDebugLayer();
        debugController->SetEnableGPUBasedValidation(TRUE);
    }

    //if (SUCCEEDED(Dx12::GetInstance()->device.Get()->QueryInterface(IID_PPV_ARGS(infoQueue.GetAddressOf()))))
    //{
    //    infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
    //    infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
    //}
#endif
#pragma endregion �f�o�b�N���̂�
    //-------------
    Dx12::GetInitInstance();
    Input* input = Input::GetInitInstance();
    Depth depth;
    ShaderResource::GetInitInstance();
    LightGroup::GetInitInstance();
    //FBXLoader::GetInitInstance();
    //���[�g�p�����[�^�̐ݒ�---------------------------
#pragma region ���[�g�p�����[�^�̐ݒ�
    RootParameter rootparams;
    rootparams.SetRootParam(D3D12_ROOT_PARAMETER_TYPE_CBV, 0, 0, D3D12_SHADER_VISIBILITY_ALL,  0);
    rootparams.SetRootParam(D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE, 0, 0, D3D12_SHADER_VISIBILITY_ALL, 1);
    rootparams.SetRootParam(D3D12_ROOT_PARAMETER_TYPE_CBV, 1, 0, D3D12_SHADER_VISIBILITY_ALL, 0);
    rootparams.SetRootParam(D3D12_ROOT_PARAMETER_TYPE_CBV, 2, 0, D3D12_SHADER_VISIBILITY_ALL, 0);
#pragma endregion ���[�g�p�����[�^�̐ݒ�
    //------------------------
 /*   PipelineRootSignature obj3dPipeline = obj3dPipeline.Create3DObjectPipeline(depth, rootparams);
    PipelineRootSignature spritePipeline = spritePipeline.CreateSpritePipeline(depth, rootparams);
    PipelineRootSignature particlePipeline = particlePipeline.CreateParticlePipeline(depth, rootparams); */
    PipeLineManager pipeline(&rootparams, &depth);
    SceneManager scene(&rootparams, &depth, &pipeline);
    scene.Initialize();


#pragma region �Q�[�����[�v
    while (true)
    {
        input->UpDateInit();
        dxWindow->messageUpdate();
        if (input->IsKeyDown(DIK_ESCAPE) || dxWindow->IsBreak()) break;
        
        scene.Update();
        scene.Draw();
    }
#pragma endregion �Q�[�����[�v
    LightGroup::DeleteInstace();
    Draw::DeleteInstace();
    ShaderResource::DeleteInstace();
    DxWindow::DeleteInstance();
    Dx12::DeleteInstace();
    Input::DeleteInstace();
    //FBXLoader::DeleteInstance();
    FPS::DeleteInstance();
}   
    TextureManager::DeleteInstace();
    _CrtDumpMemoryLeaks();
	return 0;
}
