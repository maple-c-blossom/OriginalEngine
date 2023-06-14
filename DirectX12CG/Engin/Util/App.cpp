#define _CRTDBG_MAP_ALLOC 
#include <crtdbg.h>
#include <cstdlib>
#include "App.h"

void MCB::App::Init()
{
    dxWindow_ = DxWindow::GetInitInstance();
    fps_ = FPS::GetInitInstance();
#ifdef _DEBUG
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController_))))
    {
        debugController_->EnableDebugLayer();
        debugController_->SetEnableGPUBasedValidation(TRUE);
    }
#endif
    Dx12::GetInitInstance();
#ifdef _DEBUG
    if (SUCCEEDED(Dx12::GetInstance()->device_.Get()->QueryInterface(IID_PPV_ARGS(infoQueue_.GetAddressOf()))))
    {
        infoQueue_->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
        infoQueue_->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
        D3D12_MESSAGE_ID denyids[] = { D3D12_MESSAGE_ID_RESOURCE_BARRIER_BEFORE_AFTER_MISMATCH, D3D12_MESSAGE_ID_GPU_BASED_VALIDATION_INCOMPATIBLE_RESOURCE_STATE };
        D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
        D3D12_INFO_QUEUE_FILTER filter{};
        filter.DenyList.NumIDs = _countof(denyids);
        filter.DenyList.pIDList = denyids;
        filter.DenyList.NumSeverities = _countof(severities);
        filter.DenyList.pSeverityList = severities;
        infoQueue_->PushStorageFilter(&filter);

    }
#endif
    input_ = Input::GetInitInstance();
    depth_ = std::make_unique<Depth>();
    ShaderResource::GetInitInstance();
    LightGroup::GetInitInstance();
    rootparams_ = std::make_unique<RootParameter>();
    rootparams_->SetRootParam(D3D12_ROOT_PARAMETER_TYPE_CBV, 0, 0, D3D12_SHADER_VISIBILITY_ALL, 0);
    rootparams_->SetRootParam(D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE, 0, 0, D3D12_SHADER_VISIBILITY_ALL, 1);
    rootparams_->SetRootParam(D3D12_ROOT_PARAMETER_TYPE_CBV, 1, 0, D3D12_SHADER_VISIBILITY_ALL, 0);
    rootparams_->SetRootParam(D3D12_ROOT_PARAMETER_TYPE_CBV, 2, 0, D3D12_SHADER_VISIBILITY_ALL, 0);
    rootparams_->SetRootParam(D3D12_ROOT_PARAMETER_TYPE_CBV, 3, 0, D3D12_SHADER_VISIBILITY_ALL, 0);
    rootparams_->SetRootParam(D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE, 0, 0, D3D12_SHADER_VISIBILITY_ALL, 1,1);
    pipeline_ = std::make_unique<PipeLineManager>(rootparams_.get(), depth_.get());
    scene_ = std::make_unique<SceneManager>(rootparams_.get(), depth_.get(), pipeline_.get());
    scene_->Initialize();
}

void MCB::App::MainLoop()
{
    while (true)
    {
        input_->UpDateInit();
        dxWindow_->messageUpdate();
        if (input_->IsKeyDown(DIK_ESCAPE) || dxWindow_->IsBreak()) break;

        scene_->Update();
        scene_->Draw();
    }
}

void MCB::App::Final()
{
    Input::AllStopVibration();
}

void MCB::App::Run()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    {
        Init();
        MainLoop();
        Final();
    }
    _CrtDumpMemoryLeaks();
}
