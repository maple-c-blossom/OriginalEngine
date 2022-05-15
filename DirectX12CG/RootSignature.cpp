#include "RootSignature.h"

void MCB::RootSignature::InitRootSignatureDesc(RootParameter& rootparams, TexSample& sample)
{
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootSignatureDesc.pParameters = &rootparams.rootparams.front(); //ルートパラメータの先頭アドレス
    rootSignatureDesc.NumParameters = rootparams.rootparams.size(); //ルートパラメータ数
    rootSignatureDesc.pStaticSamplers = &sample.samplerDesc;
    rootSignatureDesc.NumStaticSamplers = 1;
}
void MCB::RootSignature::SetSerializeRootSignature(Shader& shader,Dx12& dx12)
{
    dx12.result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &shader.errorBlob);
    assert(SUCCEEDED(dx12.result));
}

void MCB::RootSignature::CreateRootSignature(Dx12 *dx12)
{
    dx12->result = dx12->device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
    assert(SUCCEEDED(dx12->result));
}

