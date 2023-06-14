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

        //��{�ϐ�������-----
    #pragma region ��{�ϐ�������

        HRESULT result_;
        Microsoft::WRL::ComPtr<ID3D12Device> device_ = nullptr;
        Microsoft::WRL::ComPtr<IDXGIFactory6> dxgiFactory_ = nullptr;
        Microsoft::WRL::ComPtr<IDXGISwapChain4> swapchain_ = nullptr;
        Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_ = nullptr;
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeaps_ = nullptr;

    #pragma endregion ��{�ϐ�������
        //--------------

        //�f�o�C�X�ϐ�-----------------------
        //�Ή����x���̔z��
        D3D_FEATURE_LEVEL levels_[4] =
        {
            D3D_FEATURE_LEVEL_12_1,
            D3D_FEATURE_LEVEL_12_0,
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
        };

        D3D_FEATURE_LEVEL featureLevel_;
        //---------------------------

        //�R�}���h�L���[
        D3D12_COMMAND_QUEUE_DESC commandQueueDesc_{};

        //�X���b�v�`�F�[��
        Microsoft::WRL::ComPtr<IDXGISwapChain1> swapchain1_;

        //�e��ݒ�
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};

        // �f�X�N���v�^�ϐ�
        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc_{};

        //�o�b�N�o�b�t�@�ϐ�
        std::vector< Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers_ = std::vector< Microsoft::WRL::ComPtr<ID3D12Resource>>(2);

        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle_;

        //�����_�[�^�[�Q�b�g�r���[�̐ݒ�
        D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_{};

        //�t�F���X�ϐ�
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
