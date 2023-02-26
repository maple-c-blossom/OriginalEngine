#include "PostEffect.h"
#include "DxWindow.h"
#include "Dx12.h"

using namespace MCB;
using namespace Microsoft::WRL;
using namespace std;
using namespace DirectX;
MCB::PostEffect::PostEffect()
{
	size = { 500,500 };
	position = { 0,0,0 };
	color = { 1,1,1,1 };
	anchorPoint = { 0,0 };
	isFlipX = false;
	isFlipY = false;
	tex = nullptr;
}

void MCB::PostEffect::Init()
{
	HRESULT result = S_FALSE;


	tex = TextureManager::GetInstance()->CreateNoTextureFileIsTexture(true);
	/*CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		(UINT)DxWindow::window_width,
		(UINT)DxWindow::window_height,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);
	result = Dx12::GetInstance()->device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
			D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&texBuff));
	assert(SUCCEEDED(result));

	{
		const UINT pixelCount = DxWindow::window_width * DxWindow::window_height;
		const UINT rowPitch = sizeof(UINT) * DxWindow::window_width;
		const UINT depthPitch = rowPitch * DxWindow::window_height;
		unique_ptr<std::array<unique_ptr<UINT>, pixelCount>> imgs = make_unique<std::array<unique_ptr<UINT>, pixelCount>>();;
		for (auto& itr : *imgs.get())
		{
			itr = make_unique<UINT>();
			*itr = 0xff0000ff;
		}

		result = texBuff->WriteToSubresource(0, nullptr, imgs.get(), rowPitch, depthPitch);
		assert(SUCCEEDED(result) && "PostEffectImgError");
	}

	D3D12_DESCRIPTOR_HEAP_DESC srvDescheapDesc = {};
	srvDescheapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescheapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescheapDesc.NumDescriptors = 1;
	result = Dx12::GetInstance()->device->CreateDescriptorHeap(&srvDescheapDesc, IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result) && "PostEffectCreateDescriptorHeapError");
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	Dx12::GetInstance()->device->CreateShaderResourceView(texBuff.Get(), &srvDesc,descHeapSRV->GetCPUDescriptorHandleForHeapStart());*/



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
        tempsprite.SpriteTransferVertexBuffer();
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
