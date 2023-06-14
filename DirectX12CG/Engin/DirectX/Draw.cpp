#include "Draw.h"
#include "FPS.h"
void MCB::Draw::SetBeforeResourceBarrier()
{
    barrierDesc_ = {};
    barrierDesc_.Transition.pResource = Dx12::GetInstance()->backBuffers_[bbIndex_].Get(); // �o�b�N�o�b�t�@���w��
    barrierDesc_.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // �\������
    barrierDesc_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // �`��
    Dx12::GetInstance()->commandList_->ResourceBarrier(1, &barrierDesc_);
}

void MCB::Draw::BeforeDraw(const Depth& depth, const PipelineRootSignature& pipeline)
{
    Dx12* dx12 = Dx12::GetInstance();
    ShaderResource* srv = ShaderResource::GetInstance();
    dx12->commandList_->SetPipelineState(pipeline.pipeline_.pipelinestate_.Get());
    dx12->commandList_->SetGraphicsRootSignature(pipeline.rootsignature_.rootsignature_.Get());


    //�v���~�e�B�u�`��̐ݒ�R�}���h�i�O�p�`���X�g�j--------------------------
    dx12->commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //SRV�q�[�v�̐ݒ�R�}���h
    dx12->commandList_->SetDescriptorHeaps(1, srv->srvHeap_.GetAddressOf());
}

void MCB::Draw::SetBeforeBbIndex()
{

    bbIndex_ = Dx12::GetInstance()->swapchain_->GetCurrentBackBufferIndex();
}

D3D12_RESOURCE_BARRIER MCB::Draw::GetResouceBarrier()
{
    return barrierDesc_;
}

uint32_t MCB::Draw::GetBbIndex()
{
    return bbIndex_;
}

void MCB::Draw::SetAfterResourceBarrier()
{
    barrierDesc_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // �`��
    barrierDesc_.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;   // �\����
    Dx12::GetInstance()->commandList_->ResourceBarrier(1, &barrierDesc_);
}

void MCB::Draw::CloseDrawCommandOrder()
{
    Dx12* dx12 = Dx12::GetInstance();
    // ���߂̃N���[�Y----------------------------------
    dx12->result_ = dx12->commandList_->Close();
    assert(SUCCEEDED(dx12->result_) && "���߃N���[�Y�i�K�ł̃G���[");
    //------------
}

MCB::Draw* MCB::Draw::GetInstance()
{

    static Draw instance;
    return &instance;
}


void MCB::Draw::SetRenderTargetView(const Depth& depth)
{
    Dx12* dx12 = Dx12::GetInstance();
    rtvHandle_ = dx12->rtvHeaps_->GetCPUDescriptorHandleForHeapStart();
    rtvHandle_.ptr += bbIndex_ * dx12->device_->GetDescriptorHandleIncrementSize(dx12->rtvHeapDesc_.Type);
    dsvHandle_ = depth.dsvHeap_->GetCPUDescriptorHandleForHeapStart();
    dx12->commandList_->OMSetRenderTargets(1, &rtvHandle_, false, &dsvHandle_);
}


void MCB::Draw::SetViewPort()
{
    Dx12* dx12 = Dx12::GetInstance();
    DxWindow* dxWindow = DxWindow::GetInstance();

    viewport_ = {};

    viewport_.Width = (FLOAT)dxWindow->sWINDOW_WIDTH_;
    viewport_.Height = (FLOAT)dxWindow->sWINDOW_HEIGHT_;
    viewport_.TopLeftX = 0;
    viewport_.TopLeftY = 0;
    viewport_.MinDepth = 0.0f;
    viewport_.MaxDepth = 1.0f;

    dx12->commandList_->RSSetViewports(1, &viewport_);
}



void MCB::Draw::ClearScreen( const float* clearColor)
{
    Dx12* dx12 = Dx12::GetInstance();
    dx12->commandList_->ClearRenderTargetView(rtvHandle_, clearColor, 0, nullptr);
    dx12->commandList_->ClearDepthStencilView(dsvHandle_, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void MCB::Draw::CommandListExecution()
{

    Dx12* dx12 = Dx12::GetInstance();
    // �R�}���h���X�g�̎��s-------------------------------------
#pragma region �R�}���h���X�g���s
    ID3D12CommandList* commandLists[] = { dx12->commandList_.Get() }; // �R�}���h���X�g�̔z��
    dx12->commandQueue_->ExecuteCommandLists(1, commandLists);

    // �o�b�t�@���t���b�v�i���\�̓��ւ�)-----------------------
    dx12->result_ = dx12->swapchain_->Present(1, 0);
    assert(SUCCEEDED(dx12->result_) && "�o�b�t�@�t���b�v�i�K�ł̃G���[");
    //-----------------

#pragma region �R�}���h���s�����҂�
    // �R�}���h���X�g�̎��s������҂�
    dx12->commandQueue_->Signal(dx12->fence_.Get(), ++dx12->fenceVal_);
    if (dx12->fence_->GetCompletedValue() != dx12->fenceVal_)
    {
        HANDLE event = CreateEvent(nullptr, false, false, nullptr);
        dx12->fence_->SetEventOnCompletion(dx12->fenceVal_, event);
        if (event)
        {
            WaitForSingleObject(event, INFINITE);
            CloseHandle(event);
        }
    }
#pragma endregion �R�}���h���s�����҂�
}

void MCB::Draw::SetScissorrect()
{
    Dx12* dx12 = Dx12::GetInstance();
    DxWindow* dxWindow = DxWindow::GetInstance();


    scissorrect_ = {};

    scissorrect_.left = 0;                                       // �؂蔲�����W��
    scissorrect_.right = scissorrect_.left + dxWindow->sWINDOW_WIDTH_;        // �؂蔲�����W�E
    scissorrect_.top = 0;                                        // �؂蔲�����W��
    scissorrect_.bottom = scissorrect_.top + dxWindow->sWINDOW_HEIGHT_;       // �؂蔲�����W��

    dx12->commandList_->RSSetScissorRects(1, &scissorrect_);
}

void MCB::Draw::ResetQueAndCommandList()
{
    Dx12* dx12 = Dx12::GetInstance();
    //�L���[���N���A
    dx12->result_ = dx12->commandAllocator_->Reset(); // �L���[���N���A
    assert(SUCCEEDED(dx12->result_) && "�L���[�N���A�i�K�ł̃G���[");

    //�ĂуR�}���h���X�g�����߂鏀��
    dx12->result_ = dx12->commandList_->Reset(dx12->commandAllocator_.Get(), nullptr);  // �ĂуR�}���h���X�g�𒙂߂鏀��
    assert(SUCCEEDED(dx12->result_) && "�R�}���h���X�g�Ē��~�����i�K�ł̃G���[");
}

void MCB::Draw::PreDraw(const Depth& depth,const PipelineRootSignature& pipeline, const float* clearColor)
{
    SetBeforeBbIndex();
    SetBeforeResourceBarrier();
    SetRenderTargetView( depth);
    ClearScreen( clearColor);
    SetViewPort();
    SetScissorrect();
    BeforeDraw( depth, pipeline);

}

void MCB::Draw::PostDraw()
{
    SetAfterResourceBarrier();

    CloseDrawCommandOrder();

    CommandListExecution();

    FPS::GetInstance()->UpDate();

    ResetQueAndCommandList();
}
