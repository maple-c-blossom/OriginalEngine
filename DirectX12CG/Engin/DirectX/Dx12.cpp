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

#pragma endregion �A�_�v�^�[�ݒ�
    //�A�_�v�^�[�ݒ肱���܂�-----------------------------------------------------------------------
}

void Dx12::SetDevice(IDXGIAdapter4* tmpAdapter)
{
    //�f�o�C�X������������----------------------------------------------------------------------------------------------------------
#pragma region �f�o�C�X����

    for (int32_t i = 0; i < _countof(levels_); i++)
    {
        //�̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
        result_ = D3D12CreateDevice(tmpAdapter, levels_[i], IID_PPV_ARGS(&device_));
        if (result_ == S_OK)
        {
            featureLevel_ = levels_[i];
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
    result_ = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));
    assert(SUCCEEDED(result_));

    //�R�}���h���X�g�𐶐�
    result_ = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr, IID_PPV_ARGS(&commandList_));
    assert(SUCCEEDED(result_));


    //�R�}���h�L���[�̐���

    result_ = device_->CreateCommandQueue(&commandQueueDesc_, IID_PPV_ARGS(&commandQueue_));
    assert(SUCCEEDED(result_));

#pragma endregion �R�}���h���X�g
    //�R�}���h���X�g�����܂�------------------------------------------------------------------------------------------
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
    //DXGI�t�@�N�g���[����----------------
#pragma region DXGI�t�@�N�g���[����

//DXGI�t�@�N�g���[�̐���
    result_ = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));
    assert(SUCCEEDED(result_));

#pragma endregion DXGI�t�@�N�g���[����
    //-------------------
}

void Dx12::SetSwapChain()
{
    DxWindow* dxWindow = DxWindow::GetInstance();
    //�X���b�v�`�F�[���̐�����������------------------------------------------------------------------
#pragma region �X���b�v�`�F�[���̐���

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



#pragma endregion �X���b�v�`�F�[���̐���
    //�X���b�v�`�F�[���̐��������܂�--------------------------------------------------------------
}

void Dx12::SetDesctiptor()
{
    //�f�X�N���v�^�q�[�v�𐶐���������--------------
#pragma region �f�X�N���v�^�q�[�v����

//�e��ݒ�

    rtvHeapDesc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc_.NumDescriptors = swapChainDesc_.BufferCount;
    device_->CreateDescriptorHeap(&rtvHeapDesc_, IID_PPV_ARGS(&rtvHeaps_));

#pragma endregion �f�X�N���v�^�q�[�v����
    //�f�X�N���v�^�q�[�v�𐶐������܂�------------

}

void Dx12::SetBackBuffer()
{
    //�_�u���o�b�t�@----------------------
#pragma region �_�u���o�b�t�@

    

    backBuffers_.resize(swapChainDesc_.BufferCount);

    for (size_t i = 0; i < backBuffers_.size(); i++)
    {
        //�X���b�v�`�F�[������o�b�t�@���擾
        swapchain_->GetBuffer((uint32_t)i, IID_PPV_ARGS(&backBuffers_[i]));
        //�ŃX�N���v�^�q�[�v�n���h�����擾
        rtvHandle_ = rtvHeaps_->GetCPUDescriptorHandleForHeapStart();
        //�����\���ŃA�h���X�������
        rtvHandle_.ptr += i * device_->GetDescriptorHandleIncrementSize(rtvHeapDesc_.Type);

        //�V�F�[�_�[�̌v�Z���ʂ�SRGB�ɕϊ����ď�������
        rtvDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        rtvDesc_.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
        //�����_�[�^�[�Q�b�g�r���[�̐���
        device_->CreateRenderTargetView(backBuffers_[i].Get(), &rtvDesc_, rtvHandle_);

    }

#pragma endregion �_�u���o�b�t�@
    //------------------
}

void Dx12::SetFence()
{
    //�t�F���X�̐���---------
#pragma region �t�F���X�̐���
    result_ = device_->CreateFence(fenceVal_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
#pragma endregion �t�F���X�̐���
    //------------
}
