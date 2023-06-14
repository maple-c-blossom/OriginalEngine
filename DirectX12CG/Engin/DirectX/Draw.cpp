#include "Draw.h"
#include "FPS.h"
void MCB::Draw::SetBeforeResourceBarrier()
{
    barrierDesc_ = {};
    barrierDesc_.Transition.pResource = Dx12::GetInstance()->backBuffers_[bbIndex_].Get(); // バックバッファを指定
    barrierDesc_.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // 表示から
    barrierDesc_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画
    Dx12::GetInstance()->commandList_->ResourceBarrier(1, &barrierDesc_);
}

void MCB::Draw::BeforeDraw(const Depth& depth, const PipelineRootSignature& pipeline)
{
    Dx12* dx12 = Dx12::GetInstance();
    ShaderResource* srv = ShaderResource::GetInstance();
    dx12->commandList_->SetPipelineState(pipeline.pipeline_.pipelinestate_.Get());
    dx12->commandList_->SetGraphicsRootSignature(pipeline.rootsignature_.rootsignature_.Get());


    //プリミティブ形状の設定コマンド（三角形リスト）--------------------------
    dx12->commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //SRVヒープの設定コマンド
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
    barrierDesc_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画
    barrierDesc_.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;   // 表示に
    Dx12::GetInstance()->commandList_->ResourceBarrier(1, &barrierDesc_);
}

void MCB::Draw::CloseDrawCommandOrder()
{
    Dx12* dx12 = Dx12::GetInstance();
    // 命令のクローズ----------------------------------
    dx12->result_ = dx12->commandList_->Close();
    assert(SUCCEEDED(dx12->result_) && "命令クローズ段階でのエラー");
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
    // コマンドリストの実行-------------------------------------
#pragma region コマンドリスト実行
    ID3D12CommandList* commandLists[] = { dx12->commandList_.Get() }; // コマンドリストの配列
    dx12->commandQueue_->ExecuteCommandLists(1, commandLists);

    // バッファをフリップ（裏表の入替え)-----------------------
    dx12->result_ = dx12->swapchain_->Present(1, 0);
    assert(SUCCEEDED(dx12->result_) && "バッファフリップ段階でのエラー");
    //-----------------

#pragma region コマンド実行完了待ち
    // コマンドリストの実行完了を待つ
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
#pragma endregion コマンド実行完了待ち
}

void MCB::Draw::SetScissorrect()
{
    Dx12* dx12 = Dx12::GetInstance();
    DxWindow* dxWindow = DxWindow::GetInstance();


    scissorrect_ = {};

    scissorrect_.left = 0;                                       // 切り抜き座標左
    scissorrect_.right = scissorrect_.left + dxWindow->sWINDOW_WIDTH_;        // 切り抜き座標右
    scissorrect_.top = 0;                                        // 切り抜き座標上
    scissorrect_.bottom = scissorrect_.top + dxWindow->sWINDOW_HEIGHT_;       // 切り抜き座標下

    dx12->commandList_->RSSetScissorRects(1, &scissorrect_);
}

void MCB::Draw::ResetQueAndCommandList()
{
    Dx12* dx12 = Dx12::GetInstance();
    //キューをクリア
    dx12->result_ = dx12->commandAllocator_->Reset(); // キューをクリア
    assert(SUCCEEDED(dx12->result_) && "キュークリア段階でのエラー");

    //再びコマンドリストをためる準備
    dx12->result_ = dx12->commandList_->Reset(dx12->commandAllocator_.Get(), nullptr);  // 再びコマンドリストを貯める準備
    assert(SUCCEEDED(dx12->result_) && "コマンドリスト再貯蓄準備段階でのエラー");
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
