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
        Microsoft::WRL::ComPtr<ID3DBlob> vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
        Microsoft::WRL::ComPtr<ID3DBlob> gsBlob = nullptr; // �W�I���g���V�F�[�_�I�u�W�F�N�g
        Microsoft::WRL::ComPtr<ID3DBlob> psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
        Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr; // �G���[�I�u�W�F�N�g

       /* std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;*/

      // ���_���C�A�E�g------------------
#pragma region ���_���C�A�E�g

        D3D12_INPUT_ELEMENT_DESC inputLayout[3] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}, // (1�s�ŏ������ق������₷��)
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},//�@���x�N�g��
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}//uv���W
        };

        D3D12_INPUT_ELEMENT_DESC fbxinputLayout[5] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}, // (1�s�ŏ������ق������₷��)
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},//�@���x�N�g��
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},//uv���W,
            {"BONEID", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},//�{�[��ID
            {"BONEWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}//�{�[���E�F�C�g
        };

        D3D12_INPUT_ELEMENT_DESC SpriteinputLayout[2] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}, // (1�s�ŏ������ق������₷��)
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}//uv���W
        };
        D3D12_INPUT_ELEMENT_DESC ParticleinputLayout[1] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}, // (1�s�ŏ������ق������₷��
        };

#pragma endregion ���_���C�A�E�g
     //--------------------


        void ShaderCompile(const wchar_t* shaderFileName, const char* entryPoint,unsigned int ShaderName);

        /*void SetInputLayout(const char* semanticName, DXGI_FORMAT format);

        void SetInputLayout(const char* semanticName, DXGI_FORMAT format, unsigned int semanticIndex,  unsigned int inputSlot, 
            unsigned int AlignedByteOffset, D3D12_INPUT_CLASSIFICATION InputSlotClass, unsigned int InstanceDataStepRate);*/

    };

}
