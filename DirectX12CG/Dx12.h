#pragma once


#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <wrl.h>
#include <vector>
#include <string>
#include "DxWindow.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#pragma region using namespace

using namespace std;

using namespace Microsoft::WRL;

#pragma endregion using namespace

class Dx12
{
public:

    //基本変数初期化-----
#pragma region 基本変数初期化

    HRESULT result;
    ComPtr<ID3D12Device> device = nullptr;
    ComPtr<IDXGIFactory6> dxgiFactory = nullptr;
    ComPtr<IDXGISwapChain4> swapchain = nullptr;
    ComPtr<ID3D12CommandAllocator> commandAllocator = nullptr;
    ComPtr<ID3D12GraphicsCommandList> commandList = nullptr;
    ComPtr<ID3D12CommandQueue> commandQueue = nullptr;
    ComPtr<ID3D12DescriptorHeap> rtvHeaps = nullptr;

#pragma endregion 基本変数初期化
    //--------------

    //デバイス変数-----------------------
    //対応レベルの配列
    D3D_FEATURE_LEVEL levels[4] =
    {
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };

    D3D_FEATURE_LEVEL featureLevel;
    //---------------------------

    //コマンドキュー
    D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};

    //スワップチェーン
    ComPtr<IDXGISwapChain1> swapchain1;

    //各種設定
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

    // デスクリプタ変数
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};

    //バックバッファ変数
    vector<ComPtr<ID3D12Resource>> backBuffers = vector<ComPtr<ID3D12Resource>>(2);

    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;

    //レンダーターゲットビューの設定
    D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};

    //フェンス変数
    ComPtr<ID3D12Fence> fence = nullptr;
    UINT64 fenceVal = 0;

    Dx12(DxWindow dxWindow);

    void SetAdapter();

    void SetDevice(IDXGIAdapter4* tmpAdapter);

    void SetCommandListAndQueue();

    void SetDXFactory();

    void SetSwapChain(DxWindow dxWindow);

    void SetDesctiptor();

    void SetBackBuffer();

    void SetFence();
};

