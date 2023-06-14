#include "RootParameter.h"

MCB::RootParameter::~RootParameter()
{
    rootparams_.clear();
}

void MCB::RootParameter::SetRootParam(const D3D12_ROOT_PARAMETER_TYPE& paramType,  int32_t ShaderRegister,
     int32_t RegisterSpace, const D3D12_SHADER_VISIBILITY& shaderVisibility,
     int32_t NumDescriptorRanges, size_t descriptorIndex)
{
    D3D12_ROOT_PARAMETER rootparam{};
    rootparam.ParameterType = paramType;//種類
    rootparam.ShaderVisibility = shaderVisibility;//全てのシェーダーから見える

    if (paramType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
    {
        auto& descriptorRange = ShaderResource::GetInstance()->descriptorRange_;
        descriptorIndex = min(static_cast<size_t>(descriptorRange.size() - 1), descriptorIndex);
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

    rootparams_.push_back(rootparam);

}

void MCB::RootParameter::SetRootParam(const D3D12_ROOT_PARAMETER_TYPE& paramType,
     int32_t ShaderRegister, size_t descriptorIndex)
{
    D3D12_ROOT_PARAMETER rootparam{};
    rootparam.ParameterType = paramType;//種類
    rootparam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てのシェーダーから見える

    if (paramType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
    {
        auto& descriptorRange = ShaderResource::GetInstance()->descriptorRange_;
        descriptorIndex = min(static_cast<size_t>(descriptorRange.size() - 1), descriptorIndex);
        rootparam.DescriptorTable.pDescriptorRanges = &descriptorRange[descriptorIndex];
        rootparam.DescriptorTable.NumDescriptorRanges = static_cast<uint32_t>(descriptorIndex);
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

    rootparams_.push_back(rootparam);
}
