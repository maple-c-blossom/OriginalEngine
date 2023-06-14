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



namespace MCB
{

    class Dx12
    {
    private:
        Dx12() {};
        Dx12(const Dx12& dx12) {};
        Dx12& operator=(const Dx12& dx12) {};
        ~Dx12() {};
    public:

        //基本変数初期化-----
    #pragma region 基本変数初期化

        HRESULT result_;
        Microsoft::WRL::ComPtr<ID3D12Device> device_ = nullptr;
        Microsoft::WRL::ComPtr<IDXGIFactory6> dxgiFactory_ = nullptr;
        Microsoft::WRL::ComPtr<IDXGISwapChain4> swapchain_ = nullptr;
        Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_ = nullptr;
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeaps_ = nullptr;

    #pragma endregion 基本変数初期化
        //--------------

        //デバイス変数-----------------------
        //対応レベルの配列
        D3D_FEATURE_LEVEL levels_[4] =
        {
            D3D_FEATURE_LEVEL_12_1,
            D3D_FEATURE_LEVEL_12_0,
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
        };

        D3D_FEATURE_LEVEL featureLevel_;
        //---------------------------

        //コマンドキュー
        D3D12_COMMAND_QUEUE_DESC commandQueueDesc_{};

        //スワップチェーン
        Microsoft::WRL::ComPtr<IDXGISwapChain1> swapchain1_;

        //各種設定
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};

        // デスクリプタ変数
        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc_{};

        //バックバッファ変数
        std::vector< Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers_ = std::vector< Microsoft::WRL::ComPtr<ID3D12Resource>>(2);

        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle_;

        //レンダーターゲットビューの設定
        D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_{};

        //フェンス変数
        Microsoft::WRL::ComPtr<ID3D12Fence> fence_ = nullptr;
        UINT64 fenceVal_ = 0;

        //Dx12(DxWindow dxWindow);

        static Dx12* GetInstance();
        static Dx12* GetInitInstance();

        void Init();

        void SetDXFactory();

        void SetAdapter();

        void SetDevice(IDXGIAdapter4* tmpAdapter);

        void SetCommandListAndQueue();

        void SetSwapChain();

        void SetDesctiptor();

        void SetBackBuffer();

        void SetFence();

    };

}
