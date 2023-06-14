#include "Vertex.h"
#include <DirectXMath.h>

using namespace DirectX;

void MCB::Vertex::CreateVertexBuffer(Dx12& dx12, const D3D12_HEAP_PROPERTIES& HeapProp, const D3D12_HEAP_FLAGS& flag,
    const D3D12_RESOURCE_DESC& Resdesc, const D3D12_RESOURCE_STATES& state)
{
    dx12.result_ = dx12.device_->CreateCommittedResource(
        &HeapProp, // �q�[�v�ݒ�
        flag,
        &Resdesc, // ���\�[�X�ݒ�
        state,
        nullptr,
        IID_PPV_ARGS(&vertBuff_));
    assert(SUCCEEDED(dx12.result_));
}

//void MCB::Vertex::SetIbView(DXGI_FORMAT format)
//{
//    ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
//    ibView.Format = format;
//    ibView.SizeInBytes = sizeIB;
//}

void MCB::Vertex::SetVbView()
{
    vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
    vbView_.SizeInBytes = sizeVB_;
    vbView_.StrideInBytes = sizeof(vertices_[0]);
}

//void MCB::Vertex::CreateIndexBuffer(Dx12 &dx12,const D3D12_HEAP_PROPERTIES& HeapProp, D3D12_HEAP_FLAGS flag,const D3D12_RESOURCE_DESC Resdesc ,D3D12_RESOURCE_STATES state)
//{
//    dx12.result = dx12.device->CreateCommittedResource(
//        &HeapProp,
//        flag,
//        &Resdesc,
//        state,
//        nullptr,
//        IID_PPV_ARGS(&indexBuff)
//    );
//
//}

//void MCB::Vertex::CalculationNormalVec()
//{
//    for (int32_t i = 0; i < _countof(boxIndices) / 3; i++)
//    {
//        //�O�p�`1���ƂɌv�Z
//
//        //�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɓ����
//        uint16_t index0 = boxIndices[i * 3 + 0];
//        uint16_t index1 = boxIndices[i * 3 + 1];
//        uint16_t index2 = boxIndices[i * 3 + 2];
//
//        //�O�p�`���\�����钸�_���W
//        XMVECTOR p0 = XMLoadFloat3(&Box[index0].pos);
//        XMVECTOR p1 = XMLoadFloat3(&Box[index1].pos);
//        XMVECTOR p2 = XMLoadFloat3(&Box[index2].pos);
//
//        //p0->p1�x�N�g���Ap0->p2�x�N�g�����v�Z�i�x�N�g���̌��Z�j
//        XMVECTOR v1 = XMVectorSubtract(p1, p0);
//        XMVECTOR v2 = XMVectorSubtract(p2, p0);
//
//        //�O�ς͗������琂���ȃx�N�g��
//        XMVECTOR normal = XMVector3Cross(v1, v2);
//
//        //���K���i��������ɂ���)
//        normal = XMVector3Normalize(normal);
//
//        //���߂��@���𒸓_�f�[�^�ɑ��
//        XMStoreFloat3(&Box[index0].normal, normal);
//        XMStoreFloat3(&Box[index1].normal, normal);
//        XMStoreFloat3(&Box[index2].normal, normal);
//
//    }
//}

//HRESULT MCB::Vertex::IndexMaping()
//{
//    HRESULT result;
//    //GPU��̃o�b�t�@�ɑΉ��������z���������擾----------------------------
//    result = indexBuff->Map(0, nullptr, (void**)&indexMap);
//    //---------------------------------------
//
//    //�S�C���f�b�N�X�ɑ΂���-------------------------
//    for (int32_t i = 0; i < _countof(boxIndices); i++)
//    {
//        indexMap[i] = boxIndices[i];
//    }
//    //-----------------------
//
//    //�q���������---------------------
//    indexBuff->Unmap(0, nullptr);
//    //------------------------
//
//    return result;
//}

HRESULT MCB::Vertex::VertexMaping()
{
    HRESULT result;

    result = vertBuff_->Map(0, nullptr, (void**)&vertMap_);
    assert(SUCCEEDED(result));

    // �S���_�ɑ΂���
    for (int32_t i = 0; i < _countof(vertices_); i++)
    {
        vertMap_[i] = vertices_[i];   // ���W���R�s�[
    }

    // �}�b�v������
    vertBuff_->Unmap(0, nullptr);

    return result;
}


