#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <vector>

namespace MCB
{
    enum ShaderNames
    {
        VS,
        PS,
        GS,
    };
    class Shader
    {
    public:
        Microsoft::WRL::ComPtr<ID3DBlob> vsBlob_ = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
        Microsoft::WRL::ComPtr<ID3DBlob> gsBlob_ = nullptr; // �W�I���g���V�F�[�_�I�u�W�F�N�g
        Microsoft::WRL::ComPtr<ID3DBlob> psBlob_ = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
        Microsoft::WRL::ComPtr<ID3DBlob> errorBlob_ = nullptr; // �G���[�I�u�W�F�N�g

       /* std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;*/

      // ���_���C�A�E�g------------------
#pragma region ���_���C�A�E�g

        D3D12_INPUT_ELEMENT_DESC inputLayout_[3] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}, // (1�s�ŏ������ق������₷��)
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},//�@���x�N�g��
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}//uv���W
        };

        D3D12_INPUT_ELEMENT_DESC fbxinputLayout_[5] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}, // (1�s�ŏ������ق������₷��)
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},//�@���x�N�g��
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},//uv���W,
            {"BONEID", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},//�{�[��ID
            {"BONEWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}//�{�[���E�F�C�g
        };

        D3D12_INPUT_ELEMENT_DESC SpriteinputLayout_[2] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}, // (1�s�ŏ������ق������₷��)
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}//uv���W
        };
        D3D12_INPUT_ELEMENT_DESC ParticleinputLayout_[1] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}, // (1�s�ŏ������ق������₷��
        };

#pragma endregion ���_���C�A�E�g
     //--------------------


        void ShaderCompile(const wchar_t* shaderFileName, const char* entryPoint, size_t ShaderName);

        /*void SetInputLayout(const char* semanticName, DXGI_FORMAT format);

        void SetInputLayout(const char* semanticName, DXGI_FORMAT format, size_t semanticIndex,  size_t inputSlot, 
            size_t AlignedByteOffset, D3D12_INPUT_CLASSIFICATION InputSlotClass, size_t InstanceDataStepRate);*/

    };

}

