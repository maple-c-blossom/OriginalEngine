#include "PostEffect.h"
#include "DxWindow.h"
#include "Dx12.h"

using namespace MCB;
using namespace Microsoft::WRL;
using namespace std;
using namespace DirectX;



MCB::PostEffect::PostEffect()
{
	size = { 0,0 };
	position = { 0,0,0 };
	color = { 1,1,1,1 };
	anchorPoint = { 0,0 };
	isFlipX = false;
	isFlipY = false;
	tex = nullptr;
	//tex[1] = nullptr;
}

void MCB::PostEffect::Init()
{
	HRESULT result = S_FALSE;
	
    ID3D12Device* device = Dx12::GetInstance()->device.Get();//毎回GetInstance呼ぶのは非効率なのでポインタ確保
	
    //for (auto& itr : tex)
    //{

    tex = TextureManager::GetInstance()->CreateNoTextureFileIsTexture(true);//レンダーテクスチャ用のテクスチャを生成(これもTextureManager管理。SRVHeap複数作ったら動かなくなったため)

        D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};//RTVDescHeap作成
        rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvDescHeapDesc.NumDescriptors = 1;
        result = device->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
        assert(SUCCEEDED(result));
        device->CreateRenderTargetView(tex->texture->texBuff.texbuff.Get(), nullptr, descHeapRTV->GetCPUDescriptorHandleForHeapStart());
    //}
    //深度バッファ作成
    CD3DX12_RESOURCE_DESC depthResDesc =
        CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT,
            DxWindow::GetInstance()->window_width,
            DxWindow::GetInstance()->window_height,
            1,0,
            1,0,
            D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
    result = device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &depthResDesc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        &CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
        IID_PPV_ARGS(&depthBuff));
    assert(SUCCEEDED(result));

    D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
    DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    DescHeapDesc.NumDescriptors = 1;
    result = device->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV));
    assert(SUCCEEDED(result));


    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    device->CreateDepthStencilView(depthBuff.Get(), &dsvDesc, descHeapDSV->GetCPUDescriptorHandleForHeapStart());

}

void MCB::PostEffect::PreDraw()
{
    ID3D12GraphicsCommandList* cmdList = Dx12::GetInstance()->commandList.Get();
        cmdList->ResourceBarrier(
        1, &CD3DX12_RESOURCE_BARRIER::Transition(tex->texture.get()->texBuff.texbuff.Get(),
            D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
            D3D12_RESOURCE_STATE_RENDER_TARGET));
    D3D12_CPU_DESCRIPTOR_HANDLE rtvH = descHeapRTV->GetCPUDescriptorHandleForHeapStart();
    D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descHeapDSV->GetCPUDescriptorHandleForHeapStart();
    cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);
    cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.f, 0.f, static_cast<FLOAT>(DxWindow::GetInstance()->window_width), static_cast<FLOAT>(DxWindow::GetInstance()->window_height)));
    cmdList->RSSetScissorRects(1, &CD3DX12_RECT(static_cast<LONG>(0.f), static_cast<LONG>(0.f), static_cast<LONG>(DxWindow::GetInstance()->window_width), static_cast<LONG>(DxWindow::GetInstance()->window_height)));
    cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
    cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

}

void MCB::PostEffect::Draw()
{

    if (&tex == nullptr) return;
    Dx12* dx12 = Dx12::GetInstance();
    ShaderResource* descriptor = ShaderResource::GetInstance();

    Sprite tempsprite = *this;

    tempsprite.position.x = 0;
    tempsprite.position.y = 0;
    tempsprite.position.z = 0;

    tempsprite.SpriteUpdate();


    D3D12_RESOURCE_DESC resdesc = tex->texture->texBuff.texbuff->GetDesc();

    tempsprite.size.x = (float)resdesc.Width;

    tempsprite.size.y = (float)resdesc.Height;


    if (size.x != 0)
    {
        tempsprite.size.x = size.x;
    }

    if (size.y != 0)
    {
        tempsprite.size.y = size.y;
    }




    if (tempsprite.size.x != this->size.x || tempsprite.size.y != this->size.y)
    {
        tempsprite.SpriteTransferVertexBuffer(/*tex->texture.get()*/);
        this->size = tempsprite.size;
    }

    //SRVヒープの先頭アドレスを取得
    D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor->srvHeap->GetGPUDescriptorHandleForHeapStart();


    srvGpuHandle.ptr += tex->texture->incrementNum * dx12->device.Get()->GetDescriptorHandleIncrementSize(descriptor->srvHeapDesc.Type);

    //SRVヒープの先頭にあるSRVをパラメータ1番に設定
    dx12->commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

    //頂点データ
    dx12->commandList->IASetVertexBuffers(0, 1, &vbView);
    //定数バッファビュー(CBV)の設定コマンド
    dx12->commandList->SetGraphicsRootConstantBufferView(0, tempsprite.constBuff->GetGPUVirtualAddress());
    //描画コマンド
    dx12->commandList->DrawInstanced(4, 1, 0, 0);

}

void MCB::PostEffect::PostDraw()
{
    ID3D12GraphicsCommandList* cmdList = Dx12::GetInstance()->commandList.Get();
    cmdList->ResourceBarrier(1,&CD3DX12_RESOURCE_BARRIER::Transition(tex->texture.get()->texBuff.texbuff.Get(),
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
        D3D12_RESOURCE_STATE_RENDER_TARGET));

}
