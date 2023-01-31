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

        HRESULT result;
        Microsoft::WRL::ComPtr<ID3D12Device> device = nullptr;
        Microsoft::WRL::ComPtr<IDXGIFactory6> dxgiFactory = nullptr;
        Microsoft::WRL::ComPtr<IDXGISwapChain4> swapchain = nullptr;
        Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator = nullptr;
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList = nullptr;
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue = nullptr;
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeaps = nullptr;

    #pragma endregion ��{�ϐ�������
        //--------------

        //�f�o�C�X�ϐ�-----------------------
        //�Ή����x���̔z��
        D3D_FEATURE_LEVEL levels[4] =
        {
            D3D_FEATURE_LEVEL_12_1,
            D3D_FEATURE_LEVEL_12_0,
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
        };

        D3D_FEATURE_LEVEL featureLevel;
        //---------------------------

        //�R�}���h�L���[
        D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};

        //�X���b�v�`�F�[��
        Microsoft::WRL::ComPtr<IDXGISwapChain1> swapchain1;

        //�e��ݒ�
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

        // �f�X�N���v�^�ϐ�
        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};

        //�o�b�N�o�b�t�@�ϐ�
        std::vector< Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers = std::vector< Microsoft::WRL::ComPtr<ID3D12Resource>>(2);

        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;

        //�����_�[�^�[�Q�b�g�r���[�̐ݒ�
        D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};

        //�t�F���X�ϐ�
        Microsoft::WRL::ComPtr<ID3D12Fence> fence = nullptr;
        UINT64 fenceVal = 0;

        //Dx12(DxWindow dxWindow);

        static Dx12* GetInstance();
        static void DeleteInstace();
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
