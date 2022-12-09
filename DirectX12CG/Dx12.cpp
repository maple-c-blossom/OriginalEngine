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
    //�A�_�v�^�[�ݒ肱������--------------------------------------------------------------------------------------------
#pragma region �A�_�v�^�[�ݒ�

    //�A�_�v�^�[�̗񋓗p------------
    vector<IDXGIAdapter4*> adapters;

    //�����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
    IDXGIAdapter4* tmpAdapter = nullptr;
    //---------------------

    for (UINT i = 0; dxgiFactory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND; i++)
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

#pragma endregion �A�_�v�^�[�ݒ�
    //�A�_�v�^�[�ݒ肱���܂�-----------------------------------------------------------------------
}

void Dx12::SetDevice(IDXGIAdapter4* tmpAdapter)
{
    //�f�o�C�X������������----------------------------------------------------------------------------------------------------------
#pragma region �f�o�C�X����

    for (int i = 0; i < _countof(levels); i++)
    {
        //�̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
        result = D3D12CreateDevice(tmpAdapter, levels[i], IID_PPV_ARGS(&device));
        if (result == S_OK)
        {
            featureLevel = levels[i];
            break;
        }
    }

#pragma endregion �f�o�C�X����
    //�f�o�C�X���������܂�----------------------------------------------------------------------

}

void Dx12::SetCommandListAndQueue()
{
    //�R�}���h���X�g��������--------------------------------------------------------------------------------------------------------
#pragma region �R�}���h���X�g

    //�R�}���h�A���P�[�^�𐶐�
    result = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
    assert(SUCCEEDED(result));

    //�R�}���h���X�g�𐶐�
    result = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList));
    assert(SUCCEEDED(result));


    //�R�}���h�L���[�̐���

    result = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
    assert(SUCCEEDED(result));

#pragma endregion �R�}���h���X�g
    //�R�}���h���X�g�����܂�------------------------------------------------------------------------------------------
}

Dx12* MCB::Dx12::GetInstance()
{
    static Dx12* instance = new Dx12;
    return instance;
}

void MCB::Dx12::DeleteInstace()
{
    delete Dx12::GetInstance();
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
    //DXGI�t�@�N�g���[����----------------
#pragma region DXGI�t�@�N�g���[����

//DXGI�t�@�N�g���[�̐���
    result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
    assert(SUCCEEDED(result));

#pragma endregion DXGI�t�@�N�g���[����
    //-------------------
}

void Dx12::SetSwapChain()
{
    DxWindow* dxWindow = DxWindow::GetInstance();
    //�X���b�v�`�F�[���̐�����������------------------------------------------------------------------
#pragma region �X���b�v�`�F�[���̐���

    swapChainDesc.Width = dxWindow->window_width;
    swapChainDesc.Height = dxWindow->window_height;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    result = dxgiFactory->CreateSwapChainForHwnd(commandQueue.Get(), dxWindow->hwnd, &swapChainDesc, nullptr, nullptr, &swapchain1);

    assert(SUCCEEDED(result));

    result = swapchain1.As(&swapchain);
    assert(SUCCEEDED(result));



#pragma endregion �X���b�v�`�F�[���̐���
    //�X���b�v�`�F�[���̐��������܂�--------------------------------------------------------------
}

void Dx12::SetDesctiptor()
{
    //�f�X�N���v�^�q�[�v�𐶐���������--------------
#pragma region �f�X�N���v�^�q�[�v����

//�e��ݒ�

    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount;
    device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeaps));

#pragma endregion �f�X�N���v�^�q�[�v����
    //�f�X�N���v�^�q�[�v�𐶐������܂�------------

}

void Dx12::SetBackBuffer()
{
    //�_�u���o�b�t�@----------------------
#pragma region �_�u���o�b�t�@

    

    backBuffers.resize(swapChainDesc.BufferCount);

    for (size_t i = 0; i < backBuffers.size(); i++)
    {
        //�X���b�v�`�F�[������o�b�t�@���擾
        swapchain->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers[i]));
        //�ŃX�N���v�^�q�[�v�n���h�����擾
        rtvHandle = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
        //�����\���ŃA�h���X�������
        rtvHandle.ptr += i * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);

        //�V�F�[�_�[�̌v�Z���ʂ�SRGB�ɕϊ����ď�������
        rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
        //�����_�[�^�[�Q�b�g�r���[�̐���
        device->CreateRenderTargetView(backBuffers[i].Get(), &rtvDesc, rtvHandle);

    }

#pragma endregion �_�u���o�b�t�@
    //------------------
}

void Dx12::SetFence()
{
    //�t�F���X�̐���---------
#pragma region �t�F���X�̐���
    result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
#pragma endregion �t�F���X�̐���
    //------------
}
