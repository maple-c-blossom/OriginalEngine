#include "App.h"

void MCB::App::Init()
{
    dxWindow = DxWindow::GetInitInstance();
    fps = FPS::GetInitInstance();
#ifdef _DEBUG
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
    {
        debugController->EnableDebugLayer();
        debugController->SetEnableGPUBasedValidation(TRUE);
    }
#endif
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
    input = Input::GetInitInstance();
    depth = std::make_unique<Depth>();
    ShaderResource::GetInitInstance();
    LightGroup::GetInitInstance();
    rootparams = std::make_unique<RootParameter>();
    rootparams->SetRootParam(D3D12_ROOT_PARAMETER_TYPE_CBV, 0, 0, D3D12_SHADER_VISIBILITY_ALL, 0);
    rootparams->SetRootParam(D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE, 0, 0, D3D12_SHADER_VISIBILITY_ALL, 1);
    rootparams->SetRootParam(D3D12_ROOT_PARAMETER_TYPE_CBV, 1, 0, D3D12_SHADER_VISIBILITY_ALL, 0);
    rootparams->SetRootParam(D3D12_ROOT_PARAMETER_TYPE_CBV, 2, 0, D3D12_SHADER_VISIBILITY_ALL, 0);
    rootparams->SetRootParam(D3D12_ROOT_PARAMETER_TYPE_CBV, 3, 0, D3D12_SHADER_VISIBILITY_ALL, 0);
    pipeline = std::make_unique<PipeLineManager>(rootparams.get(), depth.get());
    scene = std::make_unique<SceneManager>(rootparams.get(), depth.get(), pipeline.get());
    scene->Initialize();
}

void MCB::App::MainLoop()
{
    while (true)
    {
        input->UpDateInit();
        dxWindow->messageUpdate();
        if (input->IsKeyDown(DIK_ESCAPE) || dxWindow->IsBreak()) break;

        scene->Update();
        scene->Draw();
    }
}

void MCB::App::Final()
{
    Input::AllStopVibration();
}

void MCB::App::Run()
{
    Init();
    MainLoop();
    Final();
}
