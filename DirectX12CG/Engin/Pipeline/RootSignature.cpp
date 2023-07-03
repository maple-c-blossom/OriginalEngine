#include "RootSignature.h"

void MCB::RootSignature::InitRootSignatureDesc(RootParameter& rootparams, TexSample& sample)
{
    rootSignatureDesc_.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootSignatureDesc_.pParameters = &rootparams.rootparams_.front(); //ルートパラメータの先頭アドレス
    rootSignatureDesc_.NumParameters = (uint32_t)rootparams.rootparams_.size(); //ルートパラメータ数
    rootSignatureDesc_.pStaticSamplers = &sample.samplerDesc_;
    rootSignatureDesc_.NumStaticSamplers = 1;
}
void MCB::RootSignature::SetSerializeRootSignature(Shader& shader)
{
    Dx12::GetInstance()->result_ = D3D12SerializeRootSignature(&rootSignatureDesc_, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob_, &shader.errorBlob_);
    //std::string error;
    //error.resize(shader.errorBlob->GetBufferSize());
    //copy_n((char*)shader.errorBlob->GetBufferPointer(),
    //    shader.errorBlob->GetBufferSize(),
    //    error.begin());
    //error += "\n";
    //// エラー内容を出力ウィンドウに表示
    //OutputDebugStringA(error.c_str());
    assert(SUCCEEDED(Dx12::GetInstance()->result_));
}

void MCB::RootSignature::CreateRootSignature()
{
    Dx12::GetInstance()->result_ = Dx12::GetInstance()->device_->CreateRootSignature(0, rootSigBlob_->GetBufferPointer(), rootSigBlob_->GetBufferSize(), IID_PPV_ARGS(&rootsignature_));
    assert(SUCCEEDED(Dx12::GetInstance()->result_));
}

