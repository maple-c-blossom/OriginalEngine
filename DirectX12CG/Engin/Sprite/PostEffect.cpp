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
}

void MCB::PostEffect::Init()
{
	HRESULT result = S_FALSE;
	
    ID3D12Device* device = Dx12::GetInstance()->device.Get();//����GetInstance�ĂԂ͔̂�����Ȃ̂Ń|�C���^�m��
	tex = TextureManager::GetInstance()->CreateNoTextureFileIsTexture(true);//�����_�[�e�N�X�`���p�̃e�N�X�`���𐶐�(�����TextureManager�Ǘ��BSRVHeap����������瓮���Ȃ��Ȃ�������)
    D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};//RTVDescHeap�쐬
    rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvDescHeapDesc.NumDescriptors = 1;
    result = device->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
    assert(SUCCEEDED(result));
    device->CreateRenderTargetView(tex->texture->texBuff.texbuff.Get(), nullptr, descHeapRTV->GetCPUDescriptorHandleForHeapStart());
    //�[�x�o�b�t�@�쐬
    CD3DX12_RESOURCE_DESC depthResDesc =
        CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT,
            DxWindow::GetInstance()->window_width,
            DxWindow::GetInstance()->window_height,
            1,0,
            1,0,
            D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
    //result = device->CreateCommittedResource(
    //    &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),

    //)

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
        tempsprite.SpriteTransferVertexBuffer(tex->texture.get());
        this->size = tempsprite.size;
    }

    //SRV�q�[�v�̐擪�A�h���X���擾
    D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor->srvHeap->GetGPUDescriptorHandleForHeapStart();


    srvGpuHandle.ptr += tex->texture->incrementNum * dx12->device.Get()->GetDescriptorHandleIncrementSize(descriptor->srvHeapDesc.Type);

    //SRV�q�[�v�̐擪�ɂ���SRV���p�����[�^1�Ԃɐݒ�
    dx12->commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

    //���_�f�[�^
    dx12->commandList->IASetVertexBuffers(0, 1, &vbView);
    //�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
    dx12->commandList->SetGraphicsRootConstantBufferView(0, tempsprite.constBuff->GetGPUVirtualAddress());
    //�`��R�}���h
    dx12->commandList->DrawInstanced(4, 1, 0, 0);

}
