#include "PIpelineRootSignature.h"
using namespace MCB;

PipelineRootSignature MCB::PipelineRootSignature::Create3DObjectPipeline( Depth& depth, RootParameter& rootparams)
{
    PipelineRootSignature pipelinerootsognature;

    pipelinerootsognature.texSample.Init();

    // ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��--------------------------------
#pragma region ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��

    pipelinerootsognature.shader.ShaderCompile(L"Resources\\Shaders\\OBJShader\\OBJVertexShader.hlsl", "main", VS);

#pragma endregion ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //------------------------------------------

     //�W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��---------------
#pragma region �W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��

    pipelinerootsognature.shader.ShaderCompile(L"Resources\\Shaders\\OBJShader\\OBJGeometryShader.hlsl", "main", GS);

#pragma endregion �W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //---------------------------------


    // �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��-------------------------------
#pragma region �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��

    pipelinerootsognature.shader.ShaderCompile(L"Resources\\Shaders\\OBJShader\\OBJPixelShader.hlsl", "main", PS);

#pragma endregion �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //--------------------------------

#pragma region ���_�V�F�[�_�ƃs�N�Z���V�F�[�_���p�C�v���C���ɐݒ�

    pipelinerootsognature.pipeline.SetGpipleneDescAll(&pipelinerootsognature.shader);

#pragma endregion ���_�V�F�[�_�ƃs�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
    //-----------------------------------

    //�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�------------------------------------
#pragma region �T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
    pipelinerootsognature.pipeline.SetSampleMask();

    pipelinerootsognature.pipeline.SetAllAddRasterizerState();
#pragma endregion �T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
    //------------------------------------


     //�u�����h�X�e�[�g�̐ݒ�-------------------------------
#pragma region �u�����h�X�e�[�g�̐ݒ�

    pipelinerootsognature.pipeline.SetRenderTaegetBlendDesc(pipelinerootsognature.pipeline.pipelineDesc.BlendState.RenderTarget[0]);

    pipelinerootsognature.pipeline.SetRenderTargetWriteMask();

    pipelinerootsognature.pipeline.SetNormalBlendDesc();

    pipelinerootsognature.pipeline.SetAlphaBlend();


#pragma endregion �u�����h�X�e�[�g�̐ݒ�
    //--------------------------

    //���_���C�A�E�g�̐ݒ�------------------
#pragma region ���_���C�A�E�g�̐ݒ�

    pipelinerootsognature.pipeline.pipelineDesc.InputLayout.pInputElementDescs = pipelinerootsognature. shader.inputLayout;
    pipelinerootsognature.pipeline.pipelineDesc.InputLayout.NumElements = _countof(pipelinerootsognature.shader.inputLayout);

#pragma endregion ���_���C�A�E�g�̐ݒ�
    //----------------------------

    //�}�`�̌`����O�p�`�ɐݒ�-------------------------
    pipelinerootsognature.pipeline.SetPrimitiveTopologyType();
    //------------------

    //���̑��̐ݒ�----------------
#pragma region ���̑��̐ݒ�

    pipelinerootsognature.pipeline.SetNumRenderTargets();
    pipelinerootsognature.pipeline.SetRTVFormats();
    pipelinerootsognature.pipeline.SetSampleDescCount();

#pragma endregion ���̑��̐ݒ�
    //----------------

    depth.SetDepthStencilState(pipelinerootsognature.pipeline.pipelineDesc);

    //���[�g�V�O�l�`���̐���--------------------------
#pragma region ���[�g�V�O�l�`���̐���


    pipelinerootsognature.rootsignature.InitRootSignatureDesc(rootparams, pipelinerootsognature.texSample);

    pipelinerootsognature.rootsignature.SetSerializeRootSignature(pipelinerootsognature.shader);

    pipelinerootsognature.rootsignature.CreateRootSignature();

    // �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g

    pipelinerootsognature.pipeline.SetRootSignature(pipelinerootsognature.rootsignature);

#pragma endregion ���[�g�V�O�l�`���̐���
    //--------------------------------

   //�p�C�v���C���X�e�[�g�̐���-------------------------
#pragma region �p�C�v���C���X�e�[�g�̐���

    pipelinerootsognature.pipeline.CreateGraphicsPipelineState();

#pragma endregion �p�C�v���C���X�e�[�g�̐���
    //-----------------------------


	return pipelinerootsognature;
}

PipelineRootSignature MCB::PipelineRootSignature::CreateSpritePipeline(Depth& depth, RootParameter& rootparams)
{

    PipelineRootSignature pipelinerootsognature;

    pipelinerootsognature.texSample.Init();

    // ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��--------------------------------
#pragma region ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��

    pipelinerootsognature.shader.ShaderCompile(L"Resources\\Shaders\\SpriteShader\\SpriteVertexShader.hlsl", "main", VS);

#pragma endregion ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //------------------------------------------

    // �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��-------------------------------
#pragma region �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��

    pipelinerootsognature.shader.ShaderCompile(L"Resources\\Shaders\\SpriteShader\\SpritePixelShader.hlsl", "main", PS);

#pragma endregion �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //--------------------------------

#pragma region ���_�V�F�[�_�ƃs�N�Z���V�F�[�_���p�C�v���C���ɐݒ�

    pipelinerootsognature.pipeline.SetSpriteGpipleneDescAll(&pipelinerootsognature.shader);

#pragma endregion ���_�V�F�[�_�ƃs�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
    //-----------------------------------

    //�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�------------------------------------
#pragma region �T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
    pipelinerootsognature.pipeline.SetSampleMask();

    pipelinerootsognature.pipeline.SetSpriteAllAddRasterizerState();

#pragma endregion �T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
    //------------------------------------


     //�u�����h�X�e�[�g�̐ݒ�-------------------------------
#pragma region �u�����h�X�e�[�g�̐ݒ�

    pipelinerootsognature.pipeline.SetRenderTaegetBlendDesc(pipelinerootsognature.pipeline.pipelineDesc.BlendState.RenderTarget[0]);

    pipelinerootsognature.pipeline.SetRenderTargetWriteMask();

    pipelinerootsognature.pipeline.SetNormalBlendDesc();

    pipelinerootsognature.pipeline.SetAlphaBlend();


#pragma endregion �u�����h�X�e�[�g�̐ݒ�
    //--------------------------

    //���_���C�A�E�g�̐ݒ�------------------
#pragma region ���_���C�A�E�g�̐ݒ�

    pipelinerootsognature.pipeline.pipelineDesc.InputLayout.pInputElementDescs = pipelinerootsognature.shader.SpriteinputLayout;
    pipelinerootsognature.pipeline.pipelineDesc.InputLayout.NumElements = _countof(pipelinerootsognature.shader.SpriteinputLayout);

#pragma endregion ���_���C�A�E�g�̐ݒ�
    //----------------------------

    //�}�`�̌`����O�p�`�ɐݒ�-------------------------
    pipelinerootsognature.pipeline.SetPrimitiveTopologyType();
    //------------------

    //���̑��̐ݒ�----------------
#pragma region ���̑��̐ݒ�

    pipelinerootsognature.pipeline.SetNumRenderTargets();
    pipelinerootsognature.pipeline.SetRTVFormats();
    pipelinerootsognature.pipeline.SetSampleDescCount();

#pragma endregion ���̑��̐ݒ�
    //----------------

    depth.SetSpriteDepthStencilState(pipelinerootsognature.pipeline.pipelineDesc);

    //���[�g�V�O�l�`���̐���--------------------------
#pragma region ���[�g�V�O�l�`���̐���


    pipelinerootsognature.rootsignature.InitRootSignatureDesc(rootparams, pipelinerootsognature.texSample);

    pipelinerootsognature.rootsignature.SetSerializeRootSignature(pipelinerootsognature.shader);

    pipelinerootsognature.rootsignature.CreateRootSignature();

    // �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g

    pipelinerootsognature.pipeline.SetRootSignature(pipelinerootsognature.rootsignature);

#pragma endregion ���[�g�V�O�l�`���̐���
    //--------------------------------

   //�p�C�v���C���X�e�[�g�̐���-------------------------
#pragma region �p�C�v���C���X�e�[�g�̐���

    pipelinerootsognature.pipeline.CreateGraphicsPipelineState();

#pragma endregion �p�C�v���C���X�e�[�g�̐���
    //-----------------------------


    return pipelinerootsognature;
}

PipelineRootSignature MCB::PipelineRootSignature::CreateParticlePipeline(Depth& depth, RootParameter& rootparams)
{

    PipelineRootSignature pipelinerootsognature;

    pipelinerootsognature.texSample.Init();

    // ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��--------------------------------
#pragma region ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��

    pipelinerootsognature.shader.ShaderCompile(L"Resources\\Shaders\\ParticleShader\\ParticleVertexShader.hlsl", "main", VS);

#pragma endregion ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //------------------------------------------

     //�W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��---------------
#pragma region �W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��

    pipelinerootsognature.shader.ShaderCompile(L"Resources\\Shaders\\ParticleShader\\ParticleGeometryShader.hlsl", "main", GS);

#pragma endregion �W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //---------------------------------


    // �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��-------------------------------
#pragma region �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��

    pipelinerootsognature.shader.ShaderCompile(L"Resources\\Shaders\\ParticleShader\\ParticlePixelShader.hlsl", "main", PS);

#pragma endregion �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //--------------------------------

#pragma region ���_�V�F�[�_�ƃs�N�Z���V�F�[�_���p�C�v���C���ɐݒ�

    pipelinerootsognature.pipeline.SetGpipleneDescAll(&pipelinerootsognature.shader);

#pragma endregion ���_�V�F�[�_�ƃs�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
    //-----------------------------------

    //�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�------------------------------------
#pragma region �T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
    pipelinerootsognature.pipeline.SetSampleMask();

    pipelinerootsognature.pipeline.SetAllAddRasterizerState();
#pragma endregion �T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
    //------------------------------------


     //�u�����h�X�e�[�g�̐ݒ�-------------------------------
#pragma region �u�����h�X�e�[�g�̐ݒ�

    pipelinerootsognature.pipeline.SetRenderTaegetBlendDesc(pipelinerootsognature.pipeline.pipelineDesc.BlendState.RenderTarget[0]);

    pipelinerootsognature.pipeline.SetRenderTargetWriteMask();

    pipelinerootsognature.pipeline.SetNormalBlendDesc();

    pipelinerootsognature.pipeline.SetAlphaBlend();


#pragma endregion �u�����h�X�e�[�g�̐ݒ�
    //--------------------------

    //���_���C�A�E�g�̐ݒ�------------------
#pragma region ���_���C�A�E�g�̐ݒ�

    pipelinerootsognature.pipeline.pipelineDesc.InputLayout.pInputElementDescs = pipelinerootsognature.shader.inputLayout;
    pipelinerootsognature.pipeline.pipelineDesc.InputLayout.NumElements = _countof(pipelinerootsognature.shader.inputLayout);

#pragma endregion ���_���C�A�E�g�̐ݒ�
    //----------------------------

    //�}�`�̌`����O�p�`�ɐݒ�-------------------------
    pipelinerootsognature.pipeline.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT);
    //------------------

    //���̑��̐ݒ�----------------
#pragma region ���̑��̐ݒ�

    pipelinerootsognature.pipeline.SetNumRenderTargets();
    pipelinerootsognature.pipeline.SetRTVFormats();
    pipelinerootsognature.pipeline.SetSampleDescCount();

#pragma endregion ���̑��̐ݒ�
    //----------------

    depth.SetDepthStencilState(pipelinerootsognature.pipeline.pipelineDesc);

    //���[�g�V�O�l�`���̐���--------------------------
#pragma region ���[�g�V�O�l�`���̐���


    pipelinerootsognature.rootsignature.InitRootSignatureDesc(rootparams, pipelinerootsognature.texSample);

    pipelinerootsognature.rootsignature.SetSerializeRootSignature(pipelinerootsognature.shader);

    pipelinerootsognature.rootsignature.CreateRootSignature();

    // �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g

    pipelinerootsognature.pipeline.SetRootSignature(pipelinerootsognature.rootsignature);

#pragma endregion ���[�g�V�O�l�`���̐���
    //--------------------------------

   //�p�C�v���C���X�e�[�g�̐���-------------------------
#pragma region �p�C�v���C���X�e�[�g�̐���

    pipelinerootsognature.pipeline.CreateGraphicsPipelineState();

#pragma endregion �p�C�v���C���X�e�[�g�̐���
    //-----------------------------


    return pipelinerootsognature;
}
