#include "RootParameter.h"

MCB::RootParameter::~RootParameter()
{
    rootparams.clear();
}

void MCB::RootParameter::SetRootParam(D3D12_ROOT_PARAMETER_TYPE paramType, int ShaderRegister, int RegisterSpace, D3D12_SHADER_VISIBILITY shaderVisibility, int NumDescriptorRanges)
{
    D3D12_ROOT_PARAMETER rootparam{};
    rootparam.ParameterType = paramType;//���
    rootparam.ShaderVisibility = shaderVisibility;//�S�ẴV�F�[�_�[���猩����

    if (paramType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
    {
        rootparam.DescriptorTable.pDescriptorRanges = &ShaderResource::GetInstance()->descriptorRange;
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

    rootparams.push_back(rootparam);

}

void MCB::RootParameter::SetRootParam(D3D12_ROOT_PARAMETER_TYPE paramType, int ShaderRegister)
{
    D3D12_ROOT_PARAMETER rootparam{};
    rootparam.ParameterType = paramType;//���
    rootparam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_�[���猩����

    if (paramType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
    {
        rootparam.DescriptorTable.pDescriptorRanges = &ShaderResource::GetInstance()->descriptorRange;
        rootparam.DescriptorTable.NumDescriptorRanges = 0;
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

    rootparams.push_back(rootparam);
}
