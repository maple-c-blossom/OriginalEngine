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

    //デバック時のみ----------
#pragma region デバック時のみ
#ifdef _DEBUG
//デバックレイヤーをオンに
    Microsoft::WRL::ComPtr<ID3D12Debug1> debugController;
    Microsoft::WRL::ComPtr<ID3D12InfoQueue> infoQueue;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
    {
        debugController->EnableDebugLayer();
        debugController->SetEnableGPUBasedValidation(TRUE);
    }



#endif
#pragma endregion デバック時のみ
    //-------------
    Dx12::GetInitInstance();
#ifdef _DEBUG
    if (SUCCEEDED(Dx12::GetInstance()->device.Get()->QueryInterface(IID_PPV_ARGS(infoQueue.GetAddressOf()))))
    {
        infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
        infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
        D3D12_MESSAGE_ID denyids[] = { D3D12_MESSAGE_ID_RESOURCE_BARRIER_BEFORE_AFTER_MISMATCH, D3D12_MESSAGE_ID_GPU_BASED_VALIDATION_INCOMPATIBLE_RESOURCE_STATE };
        D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
        D3D12_INFO_QUEUE_FILTER filter{};
        filter.DenyList.NumIDs = _countof(denyids);
        filter.DenyList.pIDList = denyids;
        filter.DenyList.NumSeverities = _countof(severities);
        filter.DenyList.pSeverityList = severities;
        infoQueue->PushStorageFilter(&filter);

    }
#endif
    Input* input = Input::GetInitInstance();
    Depth depth;
    ShaderResource::GetInitInstance();
    LightGroup::GetInitInstance();
    //FBXLoader::GetInitInstance();
    //ルートパラメータの設定---------------------------
#pragma region ルートパラメータの設定
    RootParameter rootparams;
    rootparams.SetRootParam(D3D12_ROOT_PARAMETER_TYPE_CBV, 0, 0, D3D12_SHADER_VISIBILITY_ALL,  0);
    rootparams.SetRootParam(D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE, 0, 0, D3D12_SHADER_VISIBILITY_ALL, 1);
    rootparams.SetRootParam(D3D12_ROOT_PARAMETER_TYPE_CBV, 1, 0, D3D12_SHADER_VISIBILITY_ALL, 0);
    rootparams.SetRootParam(D3D12_ROOT_PARAMETER_TYPE_CBV, 2, 0, D3D12_SHADER_VISIBILITY_ALL, 0);
    rootparams.SetRootParam(D3D12_ROOT_PARAMETER_TYPE_CBV, 3, 0, D3D12_SHADER_VISIBILITY_ALL, 0);
#pragma endregion ルートパラメータの設定
    //------------------------
 /*   PipelineRootSignature obj3dPipeline = obj3dPipeline.Create3DObjectPipeline(depth, rootparams);
    PipelineRootSignature spritePipeline = spritePipeline.CreateSpritePipeline(depth, rootparams);
    PipelineRootSignature particlePipeline = particlePipeline.CreateParticlePipeline(depth, rootparams); */
    PipeLineManager pipeline(&rootparams, &depth);
    SceneManager scene(&rootparams, &depth, &pipeline);
    scene.Initialize();


#pragma region ゲームループ
    while (true)
    {
        input->UpDateInit();
        dxWindow->messageUpdate();
        if (input->IsKeyDown(DIK_ESCAPE) || dxWindow->IsBreak()) break;
        
        scene.Update();
        scene.Draw();
    }
#pragma endregion ゲームループ
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
