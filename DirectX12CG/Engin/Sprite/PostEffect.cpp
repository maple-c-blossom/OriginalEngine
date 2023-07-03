#include "PostEffect.h"
#include "DxWindow.h"
#include "Dx12.h"

using namespace MCB;
using namespace Microsoft::WRL;
using namespace std;
using namespace DirectX;



MCB::PostEffect::PostEffect()
{
	size_ = { 0,0 };
	position_ = { 0,0,0 };
	color_ = { 1,1,1,1 };
	anchorPoint_ = { 0,0 };
	isFlipX_ = false;
	isFlipY_ = false;
	tex_[0] = nullptr;
	tex_[1] = nullptr;
}

MCB::PostEffect::~PostEffect()
{
    //for (auto& itr: tex)
    //{
    //    itr->free = true;
    //}
}

void MCB::PostEffect::Init()
{
	HRESULT result = S_FALSE;
	
    ID3D12Device* device = Dx12::GetInstance()->device_.Get();//毎回GetInstance呼ぶのは非効率なのでポインタ確保
    int32_t i = 0;
    D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};//RTVDescHeap作成
    rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvDescHeapDesc.NumDescriptors = 2;
    result = device->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV_));
    assert(SUCCEEDED(result));
    for (auto& itr : tex_)
    {
        itr = TextureManager::GetInstance()->CreateNoTextureFileIsTexture(true);//レンダーテクスチャ用のテクスチャを生成(これもTextureManager管理。SRVHeap複数作ったら動かなくなったため)
        device->CreateRenderTargetView(itr->texture->texBuff_.texbuff_.Get(), nullptr, CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapRTV_->GetCPUDescriptorHandleForHeapStart(), i, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)));
        i++;
    }
    //深度バッファ作成
    CD3DX12_RESOURCE_DESC depthResDesc =
        CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT,
            DxWindow::GetInstance()->sWINDOW_WIDTH_,
            DxWindow::GetInstance()->sWINDOW_HEIGHT_,
            1,0,
            1,0,
            D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
    result = device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &depthResDesc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        &CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
        IID_PPV_ARGS(&depthBuff_));
    assert(SUCCEEDED(result));

    D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
    DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    DescHeapDesc.NumDescriptors = 1;
    result = device->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV_));
    assert(SUCCEEDED(result));


    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    device->CreateDepthStencilView(depthBuff_.Get(), &dsvDesc, descHeapDSV_->GetCPUDescriptorHandleForHeapStart());

}

void MCB::PostEffect::PreDraw()
{
    ID3D12GraphicsCommandList* cmdList = Dx12::GetInstance()->commandList_.Get();
    ID3D12Device* device = Dx12::GetInstance()->device_.Get();
    for (auto& itr : tex_)
    {
        cmdList->ResourceBarrier(
            1, &CD3DX12_RESOURCE_BARRIER::Transition(itr->texture.get()->texBuff_.texbuff_.Get(),
                D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
                D3D12_RESOURCE_STATE_RENDER_TARGET));
    }
    array<D3D12_CPU_DESCRIPTOR_HANDLE, 2> rtvH;
    int32_t i = 0;
    for (auto& itr : rtvH)
    {
        itr = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapRTV_->GetCPUDescriptorHandleForHeapStart(), i, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
        i++;
    }
    D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descHeapDSV_->GetCPUDescriptorHandleForHeapStart();
    array<CD3DX12_VIEWPORT, 2> viewports;
    array<CD3DX12_RECT, 2> rects;
    for (auto& itr : viewports)itr = CD3DX12_VIEWPORT(0.f, 0.f, static_cast<FLOAT>(DxWindow::GetInstance()->sWINDOW_WIDTH_), static_cast<FLOAT>(DxWindow::GetInstance()->sWINDOW_HEIGHT_));
    for (auto& itr : rects)itr = CD3DX12_RECT(static_cast<LONG>(0.f), static_cast<LONG>(0.f), static_cast<LONG>(DxWindow::GetInstance()->sWINDOW_WIDTH_), static_cast<LONG>(DxWindow::GetInstance()->sWINDOW_HEIGHT_));
    
    cmdList->OMSetRenderTargets(2, &rtvH.front(), true, &dsvH);
    cmdList->RSSetViewports(2, &viewports.front());
    cmdList->RSSetScissorRects(2, &rects.front());
    for (auto& itr : rtvH) cmdList->ClearRenderTargetView(itr, CLEAR_COLOR_, 0, nullptr);
    cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

}

void MCB::PostEffect::Draw()
{

    if (&tex_ == nullptr) return;
    Dx12* dx12 = Dx12::GetInstance();
    ShaderResource* descriptor = ShaderResource::GetInstance();

    Sprite tempsprite = *this;

    tempsprite.position_.x_ = 0;
    tempsprite.position_.y_ = 0;
    tempsprite.position_.z_ = 0;

    tempsprite.SpriteUpdate();


    D3D12_RESOURCE_DESC resdesc = tex_[0]->texture->texBuff_.texbuff_->GetDesc();

    tempsprite.size_.x_ = (float)resdesc.Width;

    tempsprite.size_.y_ = (float)resdesc.Height;


    if (size_.x_ != 0)
    {
        tempsprite.size_.x_ = size_.x_;
    }

    if (size_.y_ != 0)
    {
        tempsprite.size_.y_ = size_.y_;
    }




    if (tempsprite.size_.x_ != size_.x_ || tempsprite.size_.y_ != size_.y_)
    {
        tempsprite.SpriteTransferVertexBuffer(/*tex->texture.get()*/);
        size_ = tempsprite.size_;
    }

    //SRVヒープの先頭アドレスを取得
    D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor->srvHeap_->GetGPUDescriptorHandleForHeapStart();
    D3D12_GPU_DESCRIPTOR_HANDLE startHeap = srvGpuHandle;
    uint32_t size = dx12->device_.Get()->GetDescriptorHandleIncrementSize(descriptor->srvHeapDesc_.Type);

    srvGpuHandle.ptr = startHeap.ptr + tex_[0]->texture->incrementNum_ * size;

    //SRVヒープの先頭にあるSRVをパラメータ1番に設定
    dx12->commandList_->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

   
    srvGpuHandle.ptr = startHeap.ptr + tex_[1]->texture->incrementNum_ * size;
    //SRVヒープの先頭にあるSRVをパラメータ1番に設定
    dx12->commandList_->SetGraphicsRootDescriptorTable(5, srvGpuHandle);

    //頂点データ
    dx12->commandList_->IASetVertexBuffers(0, 1, &vbView_);
    //定数バッファビュー(CBV)の設定コマンド
    dx12->commandList_->SetGraphicsRootConstantBufferView(0, tempsprite.constBuff_->GetGPUVirtualAddress());
    //描画コマンド
    dx12->commandList_->DrawInstanced(4, 1, 0, 0);

}

void MCB::PostEffect::PostDraw()
{
    ID3D12GraphicsCommandList* cmdList = Dx12::GetInstance()->commandList_.Get();
    for(auto&itr:tex_) cmdList->ResourceBarrier(1,&CD3DX12_RESOURCE_BARRIER::Transition(itr->texture.get()->texBuff_.texbuff_.Get(),D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,D3D12_RESOURCE_STATE_RENDER_TARGET));

}
