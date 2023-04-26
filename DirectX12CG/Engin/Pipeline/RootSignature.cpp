#include "RootSignature.h"

void MCB::RootSignature::InitRootSignatureDesc(RootParameter& rootparams, TexSample& sample)
{
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootSignatureDesc.pParameters = &rootparams.rootparams.front(); //ルートパラメータの先頭アドレス
    rootSignatureDesc.NumParameters = (UINT)rootparams.rootparams.size(); //ルートパラメータ数
    rootSignatureDesc.pStaticSamplers = &sample.samplerDesc;
    rootSignatureDesc.NumStaticSamplers = 1;
}
void MCB::RootSignature::SetSerializeRootSignature(Shader& shader)
{
    Dx12::GetInstance()->result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &shader.errorBlob);
    //std::string error;
    //error.resize(shader.errorBlob->GetBufferSize());
    //copy_n((char*)shader.errorBlob->GetBufferPointer(),
    //    shader.errorBlob->GetBufferSize(),
    //    error.begin());
    //error += "\n";
    //// エラー内容を出力ウィンドウに表示
    //OutputDebugStringA(error.c_str());
    assert(SUCCEEDED(Dx12::GetInstance()->result));
}

void MCB::RootSignature::CreateRootSignature()
{
    Dx12::GetInstance()->result = Dx12::GetInstance()->device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
    assert(SUCCEEDED(Dx12::GetInstance()->result));
}

