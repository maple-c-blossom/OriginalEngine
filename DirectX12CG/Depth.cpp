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
    depthResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    depthResDesc.Width = dxWindow->window_width;
    depthResDesc.Height = dxWindow->window_height;
    depthResDesc.DepthOrArraySize = 1;
    depthResDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthResDesc.SampleDesc.Count = 1;
    depthResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
}

void Depth::InitDepthHeapProp()
{
    depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
}

void Depth::InitDepthClearValue()
{
    depthClearValue.DepthStencil.Depth = 1.0f;
    depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;
}

void Depth::InitDepthBuffer()
{
    Dx12* dx12 = Dx12::GetInstance();
    dx12->result = dx12->device->CreateCommittedResource(&depthHeapProp, D3D12_HEAP_FLAG_NONE, &depthResDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &depthClearValue, IID_PPV_ARGS(&depthBuffer));
    assert(SUCCEEDED(dx12->result) && "InitDepthBuffer関数でエラー");
}

void Depth::InitDepthDescriptorHeap()
{
    Dx12* dx12 = Dx12::GetInstance();
    dsvHeapDesc.NumDescriptors = 1;
    dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

    dx12->result = dx12->device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
    assert(SUCCEEDED(dx12->result) && "InitDepthDescriptorHeap関数でエラー");
}

void Depth::InitDepthStencilView()
{
    Dx12* dx12 = Dx12::GetInstance();
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    dx12->device->CreateDepthStencilView(depthBuffer.Get(), &dsvDesc, dsvHeap->GetCPUDescriptorHandleForHeapStart());
    assert(SUCCEEDED(dx12->result) && "InitDepthStencilView関数でエラー");
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