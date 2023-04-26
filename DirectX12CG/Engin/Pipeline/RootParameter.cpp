#include "RootParameter.h"

MCB::RootParameter::~RootParameter()
{
    rootparams.clear();
}

void MCB::RootParameter::SetRootParam(D3D12_ROOT_PARAMETER_TYPE paramType, int ShaderRegister, int RegisterSpace, D3D12_SHADER_VISIBILITY shaderVisibility, int NumDescriptorRanges,unsigned int descriptorIndex)
{
    D3D12_ROOT_PARAMETER rootparam{};
    rootparam.ParameterType = paramType;//種類
    rootparam.ShaderVisibility = shaderVisibility;//全てのシェーダーから見える

    if (paramType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
    {
        auto& descriptorRange = ShaderResource::GetInstance()->descriptorRange;
        descriptorIndex = min(static_cast<unsigned int>(descriptorRange.size() - 1), descriptorIndex);
        rootparam.DescriptorTable.pDescriptorRanges = &descriptorRange[descriptorIndex];
        rootparam.DescriptorTable.NumDescriptorRanges = NumDescriptorRanges;
    }
    else if (paramType == D3D12_ROOT_PARAMETER_TYPE_CBV)
    {
        rootparam.Descriptor.ShaderRegister = ShaderRegister;//定数バッファ番号
        rootparam.Descriptor.RegisterSpace = RegisterSpace;//デフォルト値
    }
    else
    {
        return;
    }

    rootparams.push_back(rootparam);

}

void MCB::RootParameter::SetRootParam(D3D12_ROOT_PARAMETER_TYPE paramType, int ShaderRegister, unsigned int descriptorIndex)
{
    D3D12_ROOT_PARAMETER rootparam{};
    rootparam.ParameterType = paramType;//種類
    rootparam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てのシェーダーから見える

    if (paramType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
    {
        auto& descriptorRange = ShaderResource::GetInstance()->descriptorRange;
        descriptorIndex = min(static_cast<unsigned int>(descriptorRange.size() - 1), descriptorIndex);
        rootparam.DescriptorTable.pDescriptorRanges = &descriptorRange[descriptorIndex];
        rootparam.DescriptorTable.NumDescriptorRanges = descriptorIndex;
    }
    else if (paramType == D3D12_ROOT_PARAMETER_TYPE_CBV)
    {
        rootparam.Descriptor.ShaderRegister = ShaderRegister;//定数バッファ番号
        rootparam.Descriptor.RegisterSpace = 0;//デフォルト値
    }
    else
    {
        return;
    }

    rootparams.push_back(rootparam);
}
