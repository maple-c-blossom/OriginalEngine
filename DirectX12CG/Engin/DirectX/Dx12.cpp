#include "Dx12.h"

using namespace MCB;
using namespace std;

//
//Dx12::Dx12(DxWindow dxWindow)
//{
//    SetDXFactory();
//
//    SetAdapter();
//
//    SetCommandListAndQueue();
//
//    SetSwapChain(dxWindow);
//
//    SetDesctiptor();
//
//    SetBackBuffer();
//
//    SetFence();
//}

void Dx12::SetAdapter()
{
    //アダプター設定ここから--------------------------------------------------------------------------------------------
#pragma region アダプター設定

    //アダプターの列挙用------------
    vector<IDXGIAdapter4*> adapters;

    //ここに特定の名前を持つアダプターオブジェクトが入る
    IDXGIAdapter4* tmpAdapter = nullptr;
    //---------------------

    for (uint32_t i = 0; dxgiFactory_->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND; i++)
    {
        adapters.push_back(tmpAdapter);
    }

    for (size_t i = 0; i < adapters.size(); i++)
    {
        DXGI_ADAPTER_DESC3 adapterDesc;
        adapters[i]->GetDesc3(&adapterDesc);

        if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
        {
            
            tmpAdapter = adapters[i];
            break;
        }
    }

    SetDevice(tmpAdapter);
    
    tmpAdapter->Release();

#pragma endregion アダプター設定
    //アダプター設定ここまで-----------------------------------------------------------------------
}

void Dx12::SetDevice(IDXGIAdapter4* tmpAdapter)
{
    //デバイス生成ここから----------------------------------------------------------------------------------------------------------
#pragma region デバイス生成

    for (int32_t i = 0; i < _countof(levels_); i++)
    {
        //採用したアダプターでデバイスを生成
        result_ = D3D12CreateDevice(tmpAdapter, levels_[i], IID_PPV_ARGS(&device_));
        if (result_ == S_OK)
        {
            featureLevel_ = levels_[i];
            break;
        }
    }

#pragma endregion デバイス生成
    //デバイス生成ここまで----------------------------------------------------------------------

}

void Dx12::SetCommandListAndQueue()
{
    //コマンドリストここから--------------------------------------------------------------------------------------------------------
#pragma region コマンドリスト

    //コマンドアロケータを生成
    result_ = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));
    assert(SUCCEEDED(result_));

    //コマンドリストを生成
    result_ = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr, IID_PPV_ARGS(&commandList_));
    assert(SUCCEEDED(result_));


    //コマンドキューの生成

    result_ = device_->CreateCommandQueue(&commandQueueDesc_, IID_PPV_ARGS(&commandQueue_));
    assert(SUCCEEDED(result_));

#pragma endregion コマンドリスト
    //コマンドリストここまで------------------------------------------------------------------------------------------
}

Dx12* MCB::Dx12::GetInstance()
{
    static Dx12 instance;
    return &instance;
}


Dx12* MCB::Dx12::GetInitInstance()
{
    Dx12* instance = Dx12::GetInstance();
    instance->Init();
    return instance;
}



void MCB::Dx12::Init()
{
    SetDXFactory();

    SetAdapter();

    SetCommandListAndQueue();

    SetSwapChain();

    SetDesctiptor();

    SetBackBuffer();

    SetFence();
}

void Dx12::SetDXFactory()
{
    //DXGIファクトリー生成----------------
#pragma region DXGIファクトリー生成

//DXGIファクトリーの生成
    result_ = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));
    assert(SUCCEEDED(result_));

#pragma endregion DXGIファクトリー生成
    //-------------------
}

void Dx12::SetSwapChain()
{
    DxWindow* dxWindow = DxWindow::GetInstance();
    //スワップチェーンの生成ここから------------------------------------------------------------------
#pragma region スワップチェーンの生成

    swapChainDesc_.Width = dxWindow->sWINDOW_WIDTH_;
    swapChainDesc_.Height = dxWindow->sWINDOW_HEIGHT_;
    swapChainDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc_.SampleDesc.Count = 1;
    swapChainDesc_.BufferUsage = DXGI_USAGE_BACK_BUFFER;
    swapChainDesc_.BufferCount = 2;
    swapChainDesc_.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc_.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    result_ = dxgiFactory_->CreateSwapChainForHwnd(commandQueue_.Get(), dxWindow->hwnd_, &swapChainDesc_, nullptr, nullptr, &swapchain1_);

    assert(SUCCEEDED(result_));

    result_ = swapchain1_.As(&swapchain_);
    assert(SUCCEEDED(result_));



#pragma endregion スワップチェーンの生成
    //スワップチェーンの生成ここまで--------------------------------------------------------------
}

void Dx12::SetDesctiptor()
{
    //デスクリプタヒープを生成ここから--------------
#pragma region デスクリプタヒープ生成

//各種設定

    rtvHeapDesc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc_.NumDescriptors = swapChainDesc_.BufferCount;
    device_->CreateDescriptorHeap(&rtvHeapDesc_, IID_PPV_ARGS(&rtvHeaps_));

#pragma endregion デスクリプタヒープ生成
    //デスクリプタヒープを生成ここまで------------

}

void Dx12::SetBackBuffer()
{
    //ダブルバッファ----------------------
#pragma region ダブルバッファ

    

    backBuffers_.resize(swapChainDesc_.BufferCount);

    for (size_t i = 0; i < backBuffers_.size(); i++)
    {
        //スワップチェーンからバッファを取得
        swapchain_->GetBuffer((uint32_t)i, IID_PPV_ARGS(&backBuffers_[i]));
        //でスクリプタヒープハンドルを取得
        rtvHandle_ = rtvHeaps_->GetCPUDescriptorHandleForHeapStart();
        //裏か表かでアドレスがずれる
        rtvHandle_.ptr += i * device_->GetDescriptorHandleIncrementSize(rtvHeapDesc_.Type);

        //シェーダーの計算結果をSRGBに変換して書き込む
        rtvDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        rtvDesc_.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
        //レンダーターゲットビューの生成
        device_->CreateRenderTargetView(backBuffers_[i].Get(), &rtvDesc_, rtvHandle_);

    }

#pragma endregion ダブルバッファ
    //------------------
}

void Dx12::SetFence()
{
    //フェンスの生成---------
#pragma region フェンスの生成
    result_ = device_->CreateFence(fenceVal_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
#pragma endregion フェンスの生成
    //------------
}
