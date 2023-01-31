#include "Draw.h"
#include "FPS.h"
void MCB::Draw::SetBeforeResourceBarrier()
{
    barrierDesc = {};
    barrierDesc.Transition.pResource = Dx12::GetInstance()->backBuffers[bbIndex].Get(); // バックバッファを指定
    barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // 表示から
    barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画
    Dx12::GetInstance()->commandList->ResourceBarrier(1, &barrierDesc);
}

void MCB::Draw::BeforeDraw( Depth depth, PipelineRootSignature pipeline)
{
    Dx12* dx12 = Dx12::GetInstance();
    ShaderResource* srv = ShaderResource::GetInstance();
    dx12->commandList->SetPipelineState(pipeline.pipeline.pipelinestate.Get());
    dx12->commandList->SetGraphicsRootSignature(pipeline.rootsignature.rootsignature.Get());


    //プリミティブ形状の設定コマンド（三角形リスト）--------------------------
    dx12->commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //SRVヒープの設定コマンド
    dx12->commandList->SetDescriptorHeaps(1, srv->srvHeap.GetAddressOf());
}

void MCB::Draw::SetBeforeBbIndex()
{

    bbIndex = Dx12::GetInstance()->swapchain->GetCurrentBackBufferIndex();
}

D3D12_RESOURCE_BARRIER MCB::Draw::GetResouceBarrier()
{
    return barrierDesc;
}

UINT MCB::Draw::GetBbIndex()
{
    return bbIndex;
}

void MCB::Draw::SetAfterResourceBarrier()
{
    barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画
    barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;   // 表示に
    Dx12::GetInstance()->commandList->ResourceBarrier(1, &barrierDesc);
}

void MCB::Draw::CloseDrawCommandOrder()
{
    Dx12* dx12 = Dx12::GetInstance();
    // 命令のクローズ----------------------------------
    dx12->result = dx12->commandList->Close();
    assert(SUCCEEDED(dx12->result) && "命令クローズ段階でのエラー");
    //------------
}

MCB::Draw* MCB::Draw::GetInstance()
{

    static Draw* instance = new Draw;
    return instance;
}


void MCB::Draw::SetRenderTargetView(Depth& depth)
{
    Dx12* dx12 = Dx12::GetInstance();
    rtvHandle = dx12->rtvHeaps->GetCPUDescriptorHandleForHeapStart();
    rtvHandle.ptr += bbIndex * dx12->device->GetDescriptorHandleIncrementSize(dx12->rtvHeapDesc.Type);
    dsvHandle = depth.dsvHeap->GetCPUDescriptorHandleForHeapStart();
    dx12->commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);
}


void MCB::Draw::SetViewPort()
{
    Dx12* dx12 = Dx12::GetInstance();
    DxWindow* dxWindow = DxWindow::GetInstance();

    viewport = {};

    viewport.Width = dxWindow->window_width;
    viewport.Height = dxWindow->window_height;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    dx12->commandList->RSSetViewports(1, &viewport);
}

void MCB::Draw::DeleteInstace()
{
    delete Draw::GetInstance();
}

void MCB::Draw::ClearScreen( const float* clearColor)
{
    Dx12* dx12 = Dx12::GetInstance();
    dx12->commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    dx12->commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void MCB::Draw::CommandListExecution()
{

    Dx12* dx12 = Dx12::GetInstance();
    // コマンドリストの実行-------------------------------------
#pragma region コマンドリスト実行
    ID3D12CommandList* commandLists[] = { dx12->commandList.Get() }; // コマンドリストの配列
    dx12->commandQueue->ExecuteCommandLists(1, commandLists);

    // バッファをフリップ（裏表の入替え)-----------------------
    dx12->result = dx12->swapchain->Present(1, 0);
    assert(SUCCEEDED(dx12->result) && "バッファフリップ段階でのエラー");
    //-----------------

#pragma region コマンド実行完了待ち
    // コマンドリストの実行完了を待つ
    dx12->commandQueue->Signal(dx12->fence.Get(), ++dx12->fenceVal);
    if (dx12->fence->GetCompletedValue() != dx12->fenceVal)
    {
        HANDLE event = CreateEvent(nullptr, false, false, nullptr);
        dx12->fence->SetEventOnCompletion(dx12->fenceVal, event);
        WaitForSingleObject(event, INFINITE);
        CloseHandle(event);
    }
#pragma endregion コマンド実行完了待ち
}

void MCB::Draw::SetScissorrect()
{
    Dx12* dx12 = Dx12::GetInstance();
    DxWindow* dxWindow = DxWindow::GetInstance();


    scissorrect = {};

    scissorrect.left = 0;                                       // 切り抜き座標左
    scissorrect.right = scissorrect.left + dxWindow->window_width;        // 切り抜き座標右
    scissorrect.top = 0;                                        // 切り抜き座標上
    scissorrect.bottom = scissorrect.top + dxWindow->window_height;       // 切り抜き座標下

    dx12->commandList->RSSetScissorRects(1, &scissorrect);
}

void MCB::Draw::ResetQueAndCommandList()
{
    Dx12* dx12 = Dx12::GetInstance();
    //キューをクリア
    dx12->result = dx12->commandAllocator->Reset(); // キューをクリア
    assert(SUCCEEDED(dx12->result) && "キュークリア段階でのエラー");

    //再びコマンドリストをためる準備
    dx12->result = dx12->commandList->Reset(dx12->commandAllocator.Get(), nullptr);  // 再びコマンドリストを貯める準備
    assert(SUCCEEDED(dx12->result) && "コマンドリスト再貯蓄準備段階でのエラー");
}

void MCB::Draw::PreDraw(Depth depth, PipelineRootSignature pipeline, const float* clearColor)
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
