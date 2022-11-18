#include "PIpelineRootSignature.h"
using namespace MCB;

void MCB::PipelineRootSignature::Create3DObjectPipeline( Depth& depth, RootParameter& rootparams,int blendMode)
{

     texSample.Init();

    // ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��--------------------------------
#pragma region ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��

     shader.ShaderCompile(L"Resources\\Shaders\\OBJShader\\OBJVertexShader.hlsl", "main", VS);

#pragma endregion ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //------------------------------------------

     //�W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��---------------
#pragma region �W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��

     shader.ShaderCompile(L"Resources\\Shaders\\OBJShader\\OBJGeometryShader.hlsl", "main", GS);

#pragma endregion �W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //---------------------------------


    // �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��-------------------------------
#pragma region �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��

     shader.ShaderCompile(L"Resources\\Shaders\\OBJShader\\OBJPixelShader.hlsl", "main", PS);

#pragma endregion �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //--------------------------------

#pragma region ���_�V�F�[�_�ƃs�N�Z���V�F�[�_���p�C�v���C���ɐݒ�

     pipeline.SetGpipleneDescAll(& shader);

#pragma endregion ���_�V�F�[�_�ƃs�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
    //-----------------------------------

    //�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�------------------------------------
#pragma region �T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
     pipeline.SetSampleMask();

     pipeline.SetAllAddRasterizerState();
#pragma endregion �T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
    //------------------------------------


     //�u�����h�X�e�[�g�̐ݒ�-------------------------------
#pragma region �u�����h�X�e�[�g�̐ݒ�

     pipeline.SetRenderTaegetBlendDesc( pipeline.pipelineDesc.BlendState.RenderTarget[0]);

     pipeline.SetRenderTargetWriteMask();

     pipeline.SetNormalBlendDesc();
    switch (blendMode)
    {
    case Alpha:
         pipeline.SetAlphaBlend();
        break;
    case Add:
         pipeline.SetAddBlend();
        break;
    case Sub:
         pipeline.SetSubBlend();
        break;
    case Inv:
         pipeline.SetInvBlend();
        break;
    default:
         pipeline.SetAlphaBlend();
        break;
    }


#pragma endregion �u�����h�X�e�[�g�̐ݒ�
    //--------------------------

    //���_���C�A�E�g�̐ݒ�------------------
#pragma region ���_���C�A�E�g�̐ݒ�

     pipeline.pipelineDesc.InputLayout.pInputElementDescs =   shader.inputLayout;
     pipeline.pipelineDesc.InputLayout.NumElements = _countof( shader.inputLayout);

#pragma endregion ���_���C�A�E�g�̐ݒ�
    //----------------------------

    //�}�`�̌`����O�p�`�ɐݒ�-------------------------
     pipeline.SetPrimitiveTopologyType();
    //------------------

    //���̑��̐ݒ�----------------
#pragma region ���̑��̐ݒ�

     pipeline.SetNumRenderTargets();
     pipeline.SetRTVFormats();
     pipeline.SetSampleDescCount();

#pragma endregion ���̑��̐ݒ�
    //----------------

    depth.SetDepthStencilState( pipeline.pipelineDesc);

    //���[�g�V�O�l�`���̐���--------------------------
#pragma region ���[�g�V�O�l�`���̐���


     rootsignature.InitRootSignatureDesc(rootparams,  texSample);

     rootsignature.SetSerializeRootSignature( shader);

     rootsignature.CreateRootSignature();

    // �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g

     pipeline.SetRootSignature( rootsignature);

#pragma endregion ���[�g�V�O�l�`���̐���
    //--------------------------------

   //�p�C�v���C���X�e�[�g�̐���-------------------------
#pragma region �p�C�v���C���X�e�[�g�̐���

     pipeline.CreateGraphicsPipelineState();

#pragma endregion �p�C�v���C���X�e�[�g�̐���
    //-----------------------------



}

void MCB::PipelineRootSignature::CreateSpritePipeline(Depth& depth, RootParameter& rootparams, int blendMode)
{



     texSample.Init();

    // ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��--------------------------------
#pragma region ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��

     shader.ShaderCompile(L"Resources\\Shaders\\SpriteShader\\SpriteVertexShader.hlsl", "main", VS);

#pragma endregion ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //------------------------------------------

    // �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��-------------------------------
#pragma region �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��

     shader.ShaderCompile(L"Resources\\Shaders\\SpriteShader\\SpritePixelShader.hlsl", "main", PS);

#pragma endregion �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //--------------------------------

#pragma region ���_�V�F�[�_�ƃs�N�Z���V�F�[�_���p�C�v���C���ɐݒ�

     pipeline.SetSpriteGpipleneDescAll(& shader);

#pragma endregion ���_�V�F�[�_�ƃs�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
    //-----------------------------------

    //�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�------------------------------------
#pragma region �T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
     pipeline.SetSampleMask();

     pipeline.SetSpriteAllAddRasterizerState();

#pragma endregion �T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
    //------------------------------------


     //�u�����h�X�e�[�g�̐ݒ�-------------------------------
#pragma region �u�����h�X�e�[�g�̐ݒ�

     pipeline.SetRenderTaegetBlendDesc( pipeline.pipelineDesc.BlendState.RenderTarget[0]);

     pipeline.SetRenderTargetWriteMask();

     pipeline.SetNormalBlendDesc();

    switch (blendMode)
    {
    case Alpha:
         pipeline.SetAlphaBlend();
        break;
    case Add:
         pipeline.SetAddBlend();
        break;
    case Sub:
         pipeline.SetSubBlend();
        break;
    case Inv:
         pipeline.SetInvBlend();
        break;
    default:
         pipeline.SetAlphaBlend();
        break;
    }

#pragma endregion �u�����h�X�e�[�g�̐ݒ�
    //--------------------------

    //���_���C�A�E�g�̐ݒ�------------------
#pragma region ���_���C�A�E�g�̐ݒ�

     pipeline.pipelineDesc.InputLayout.pInputElementDescs =  shader.SpriteinputLayout;
     pipeline.pipelineDesc.InputLayout.NumElements = _countof( shader.SpriteinputLayout);

#pragma endregion ���_���C�A�E�g�̐ݒ�
    //----------------------------

    //�}�`�̌`����O�p�`�ɐݒ�-------------------------
     pipeline.SetPrimitiveTopologyType();
    //------------------

    //���̑��̐ݒ�----------------
#pragma region ���̑��̐ݒ�

     pipeline.SetNumRenderTargets();
     pipeline.SetRTVFormats();
     pipeline.SetSampleDescCount();

#pragma endregion ���̑��̐ݒ�
    //----------------

    depth.SetSpriteDepthStencilState( pipeline.pipelineDesc);

    //���[�g�V�O�l�`���̐���--------------------------
#pragma region ���[�g�V�O�l�`���̐���


     rootsignature.InitRootSignatureDesc(rootparams,  texSample);

     rootsignature.SetSerializeRootSignature( shader);

     rootsignature.CreateRootSignature();

    // �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g

     pipeline.SetRootSignature( rootsignature);

#pragma endregion ���[�g�V�O�l�`���̐���
    //--------------------------------

   //�p�C�v���C���X�e�[�g�̐���-------------------------
#pragma region �p�C�v���C���X�e�[�g�̐���

     pipeline.CreateGraphicsPipelineState();

#pragma endregion �p�C�v���C���X�e�[�g�̐���
    //-----------------------------


}

void MCB::PipelineRootSignature::CreateParticlePipeline(Depth& depth, RootParameter& rootparams, int blendMode)
{


     texSample.Init();

    // ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��--------------------------------
#pragma region ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��

     shader.ShaderCompile(L"Resources\\Shaders\\ParticleShader\\ParticleVertexShader.hlsl", "main", VS);

#pragma endregion ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //------------------------------------------

     //�W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��---------------
#pragma region �W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��

     shader.ShaderCompile(L"Resources\\Shaders\\ParticleShader\\ParticleGeometryShader.hlsl", "main", GS);

#pragma endregion �W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //---------------------------------


    // �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��-------------------------------
#pragma region �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��

     shader.ShaderCompile(L"Resources\\Shaders\\ParticleShader\\ParticlePixelShader.hlsl", "main", PS);

#pragma endregion �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //--------------------------------

#pragma region ���_�V�F�[�_�ƃs�N�Z���V�F�[�_���p�C�v���C���ɐݒ�

     pipeline.SetGpipleneDescAll(& shader);

#pragma endregion ���_�V�F�[�_�ƃs�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
    //-----------------------------------

    //�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�------------------------------------
#pragma region �T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
     pipeline.SetSampleMask();

     pipeline.SetAllAddRasterizerState();
#pragma endregion �T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
    //------------------------------------


     //�u�����h�X�e�[�g�̐ݒ�-------------------------------
#pragma region �u�����h�X�e�[�g�̐ݒ�

     pipeline.SetRenderTaegetBlendDesc( pipeline.pipelineDesc.BlendState.RenderTarget[0]);

     pipeline.SetRenderTargetWriteMask();

     pipeline.SetNormalBlendDesc();
    switch (blendMode)
    {
    case Alpha:
         pipeline.SetAlphaBlend();
        break;
    case Add:
         pipeline.SetAddBlend();
        break;
    case Sub:
         pipeline.SetSubBlend();
        break;
    case Inv:
         pipeline.SetInvBlend();
        break;
    default:
         pipeline.SetAlphaBlend();
        break;
    }
#pragma endregion �u�����h�X�e�[�g�̐ݒ�
    //--------------------------

    //���_���C�A�E�g�̐ݒ�------------------
#pragma region ���_���C�A�E�g�̐ݒ�

     pipeline.pipelineDesc.InputLayout.pInputElementDescs =  shader.inputLayout;
     pipeline.pipelineDesc.InputLayout.NumElements = _countof( shader.inputLayout);

#pragma endregion ���_���C�A�E�g�̐ݒ�
    //----------------------------

    //�}�`�̌`����O�p�`�ɐݒ�-------------------------
     pipeline.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT);
    //------------------

    //���̑��̐ݒ�----------------
#pragma region ���̑��̐ݒ�

     pipeline.SetNumRenderTargets();
     pipeline.SetRTVFormats();
     pipeline.SetSampleDescCount();

#pragma endregion ���̑��̐ݒ�
    //----------------

    depth.SetParticleDepthStencilState( pipeline.pipelineDesc);

    //���[�g�V�O�l�`���̐���--------------------------
#pragma region ���[�g�V�O�l�`���̐���


     rootsignature.InitRootSignatureDesc(rootparams,  texSample);

     rootsignature.SetSerializeRootSignature( shader);

     rootsignature.CreateRootSignature();

    // �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g

     pipeline.SetRootSignature( rootsignature);

#pragma endregion ���[�g�V�O�l�`���̐���
    //--------------------------------

   //�p�C�v���C���X�e�[�g�̐���-------------------------
#pragma region �p�C�v���C���X�e�[�g�̐���

     pipeline.CreateGraphicsPipelineState();

#pragma endregion �p�C�v���C���X�e�[�g�̐���
    //-----------------------------



}

void MCB::PipelineRootSignature::SetBrendMode(int blendMode)
{
    switch (blendMode)
    {
    case Alpha:
        pipeline.SetAlphaBlend();
        break;
    case Add:
        pipeline.SetAddBlend();
        break;
    case Sub:
        pipeline.SetSubBlend();
        break;
    case Inv:
        pipeline.SetInvBlend();
        break;
    default:
        pipeline.SetAlphaBlend();
        break;
    }
}

void MCB::PipelineRootSignature::CommonBeginDraw(bool toporogyTypeIsPoint)
{
    Dx12* dx12 = Dx12::GetInstance();
    dx12->commandList->SetPipelineState(this->pipeline.pipelinestate.Get());
    dx12->commandList->SetGraphicsRootSignature(this->rootsignature.rootsignature.Get());

    if (toporogyTypeIsPoint)
    {
        dx12->commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
    }
    else
    {
        dx12->commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    }
    //�v���~�e�B�u�`��̐ݒ�R�}���h�i�O�p�`���X�g�j--------------------------

    //SRV�q�[�v�̐ݒ�R�}���h
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> ppHeaps[] = { ShaderResource::GetInstance()->srvHeap };
    dx12->commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps->GetAddressOf());
}
