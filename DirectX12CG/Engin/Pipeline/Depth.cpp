#include "Depth.h"

using namespace MCB;
Depth::Depth()
{
    InitDepthResDesc();
    InitDepthHeapProp();
    InitDepthClearValue();
    InitDepthBuffer();
    InitDepthDescriptorHeap();
    InitDepthStencilView();
}

void Depth::InitDepthResDesc()
{
    DxWindow* dxWindow = DxWindow::GetInstance();
    depthResDesc_.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    depthResDesc_.Width = dxWindow->sWINDOW_WIDTH_;
    depthResDesc_.Height = dxWindow->sWINDOW_HEIGHT_;
    depthResDesc_.DepthOrArraySize = 1;
    depthResDesc_.Format = DXGI_FORMAT_D32_FLOAT;
    depthResDesc_.SampleDesc.Count = 1;
    depthResDesc_.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
}

void Depth::InitDepthHeapProp()
{
    depthHeapProp_.Type = D3D12_HEAP_TYPE_DEFAULT;
}

void Depth::InitDepthClearValue()
{
    depthClearValue_.DepthStencil.Depth = 1.0f;
    depthClearValue_.Format = DXGI_FORMAT_D32_FLOAT;
}

void Depth::InitDepthBuffer()
{
    Dx12* dx12 = Dx12::GetInstance();
    dx12->result_ = dx12->device_->CreateCommittedResource(&depthHeapProp_, D3D12_HEAP_FLAG_NONE, &depthResDesc_, D3D12_RESOURCE_STATE_DEPTH_WRITE, &depthClearValue_, IID_PPV_ARGS(&depthBuffer_));
    assert(SUCCEEDED(dx12->result_) && "InitDepthBuffer関数でエラー");
}

void Depth::InitDepthDescriptorHeap()
{
    Dx12* dx12 = Dx12::GetInstance();
    dsvHeapDesc_.NumDescriptors = 1;
    dsvHeapDesc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

    dx12->result_ = dx12->device_->CreateDescriptorHeap(&dsvHeapDesc_, IID_PPV_ARGS(&dsvHeap_));
    assert(SUCCEEDED(dx12->result_) && "InitDepthDescriptorHeap関数でエラー");
}

void Depth::InitDepthStencilView()
{
    Dx12* dx12 = Dx12::GetInstance();
    dsvDesc_.Format = DXGI_FORMAT_D32_FLOAT;
    dsvDesc_.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    dx12->device_->CreateDepthStencilView(depthBuffer_.Get(), &dsvDesc_, dsvHeap_->GetCPUDescriptorHandleForHeapStart());
    assert(SUCCEEDED(dx12->result_) && "InitDepthStencilView関数でエラー");
}

void Depth::SetDepthStencilState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc)
{
    pipelineDesc.DepthStencilState.DepthEnable = true;
    pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
    pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
}

void Depth::SetSpriteDepthStencilState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc)
{
    pipelineDesc.DepthStencilState.DepthEnable = false;
    pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
    pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
}

void Depth::SetParticleDepthStencilState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc)
{
    pipelineDesc.DepthStencilState.DepthEnable = true;
    pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
    pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
    pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
}