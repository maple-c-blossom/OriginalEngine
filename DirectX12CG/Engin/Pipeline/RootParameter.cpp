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
    rootparam.ParameterType = paramType;//���
    rootparam.ShaderVisibility = shaderVisibility;//�S�ẴV�F�[�_�[���猩����

    if (paramType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
    {
        auto& descriptorRange = ShaderResource::GetInstance()->descriptorRange_;
        descriptorIndex = min(static_cast<size_t>(descriptorRange.size() - 1), descriptorIndex);
        rootparam.DescriptorTable.pDescriptorRanges = &descriptorRange[descriptorIndex];
        rootparam.DescriptorTable.NumDescriptorRanges = NumDescriptorRanges;
    }
    else if (paramType == D3D12_ROOT_PARAMETER_TYPE_CBV)
    {
        rootparam.Descriptor.ShaderRegister = ShaderRegister;//�萔�o�b�t�@�ԍ�
        rootparam.Descriptor.RegisterSpace = RegisterSpace;//�f�t�H���g�l
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
    rootparam.ParameterType = paramType;//���
    rootparam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_�[���猩����

    if (paramType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
    {
        auto& descriptorRange = ShaderResource::GetInstance()->descriptorRange_;
        descriptorIndex = min(static_cast<size_t>(descriptorRange.size() - 1), descriptorIndex);
        rootparam.DescriptorTable.pDescriptorRanges = &descriptorRange[descriptorIndex];
        rootparam.DescriptorTable.NumDescriptorRanges = static_cast<uint32_t>(descriptorIndex);
    }
    else if (paramType == D3D12_ROOT_PARAMETER_TYPE_CBV)
    {
        rootparam.Descriptor.ShaderRegister = ShaderRegister;//�萔�o�b�t�@�ԍ�
        rootparam.Descriptor.RegisterSpace = 0;//�f�t�H���g�l
    }
    else
    {
        return;
    }

    rootparams_.push_back(rootparam);
}
