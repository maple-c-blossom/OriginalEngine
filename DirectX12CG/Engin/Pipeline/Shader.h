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
        Microsoft::WRL::ComPtr<ID3DBlob> vsBlob_ = nullptr; // 頂点シェーダオブジェクト
        Microsoft::WRL::ComPtr<ID3DBlob> gsBlob_ = nullptr; // ジオメトリシェーダオブジェクト
        Microsoft::WRL::ComPtr<ID3DBlob> psBlob_ = nullptr; // ピクセルシェーダオブジェクト
        Microsoft::WRL::ComPtr<ID3DBlob> errorBlob_ = nullptr; // エラーオブジェクト

       /* std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;*/

      // 頂点レイアウト------------------
#pragma region 頂点レイアウト

        D3D12_INPUT_ELEMENT_DESC inputLayout_[3] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}, // (1行で書いたほうが見やすい)
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},//法線ベクトル
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}//uv座標
        };

        D3D12_INPUT_ELEMENT_DESC fbxinputLayout_[5] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}, // (1行で書いたほうが見やすい)
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},//法線ベクトル
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},//uv座標,
            {"BONEID", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},//ボーンID
            {"BONEWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}//ボーンウェイト
        };

        D3D12_INPUT_ELEMENT_DESC SpriteinputLayout_[2] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}, // (1行で書いたほうが見やすい)
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}//uv座標
        };
        D3D12_INPUT_ELEMENT_DESC ParticleinputLayout_[1] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}, // (1行で書いたほうが見やすい
        };

#pragma endregion 頂点レイアウト
     //--------------------


        void ShaderCompile(const wchar_t* shaderFileName, const char* entryPoint, size_t ShaderName);

        /*void SetInputLayout(const char* semanticName, DXGI_FORMAT format);

        void SetInputLayout(const char* semanticName, DXGI_FORMAT format, size_t semanticIndex,  size_t inputSlot, 
            size_t AlignedByteOffset, D3D12_INPUT_CLASSIFICATION InputSlotClass, size_t InstanceDataStepRate);*/

    };

}

