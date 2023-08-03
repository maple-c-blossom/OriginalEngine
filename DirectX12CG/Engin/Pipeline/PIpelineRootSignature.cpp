#include "PIpelineRootSignature.h"
using namespace MCB;

void MCB::PipelineRootSignature::Create3DObjectPipeline( Depth& depth, RootParameter& rootparams, uint16_t blendMode)
{

     texSample_.Init();

    // ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��--------------------------------
#pragma region ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��

     shader_.ShaderCompile(L"Resources\\Shaders\\OBJShader\\OBJVertexShader.hlsl", "main", VS);

#pragma endregion ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //------------------------------------------

     //�W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��---------------
#pragma region �W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��

     shader_.ShaderCompile(L"Resources\\Shaders\\OBJShader\\OBJGeometryShader.hlsl", "main", GS);

#pragma endregion �W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //---------------------------------


    // �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��-------------------------------
#pragma region �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��

     shader_.ShaderCompile(L"Resources\\Shaders\\OBJShader\\OBJPixelShader.hlsl", "main", PS);

#pragma endregion �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //--------------------------------

#pragma region ���_�V�F�[�_�ƃs�N�Z���V�F�[�_���p�C�v���C���ɐݒ�

     pipeline_.SetGpipleneDescAll(& shader_);

#pragma endregion ���_�V�F�[�_�ƃs�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
    //-----------------------------------

    //�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�------------------------------------
#pragma region �T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
     pipeline_.SetSampleMask();

     pipeline_.SetAllAddRasterizerState();
#pragma endregion �T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
    //------------------------------------


     //�u�����h�X�e�[�g�̐ݒ�-------------------------------
#pragma region �u�����h�X�e�[�g�̐ݒ�

     pipeline_.SetRenderTaegetBlendDesc( pipeline_.pipelineDesc_.BlendState.RenderTarget[0]);

     pipeline_.SetRenderTargetWriteMask();

     pipeline_.SetNormalBlendDesc();
    switch (blendMode)
    {
    case Alpha:
         pipeline_.SetAlphaBlend();
        break;
    case Add:
         pipeline_.SetAddBlend();
        break;
    case Sub:
         pipeline_.SetSubBlend();
        break;
    case Inv:
         pipeline_.SetInvBlend();
        break;
    default:
         pipeline_.SetAlphaBlend();
        break;
    }


#pragma endregion �u�����h�X�e�[�g�̐ݒ�
    //--------------------------

    //���_���C�A�E�g�̐ݒ�------------------
#pragma region ���_���C�A�E�g�̐ݒ�

     pipeline_.pipelineDesc_.InputLayout.pInputElementDescs =   shader_.inputLayout_;
     pipeline_.pipelineDesc_.InputLayout.NumElements = _countof( shader_.inputLayout_);

#pragma endregion ���_���C�A�E�g�̐ݒ�
    //----------------------------

    //�}�`�̌`����O�p�`�ɐݒ�-------------------------
     pipeline_.SetPrimitiveTopologyType();
    //------------------

    //���̑��̐ݒ�----------------
#pragma region ���̑��̐ݒ�


     pipeline_.SetNumRenderTargets(2);
     pipeline_.SetRTVFormats(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 0);
     pipeline_.SetRTVFormats(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 1);
     pipeline_.SetSampleDescCount();

#pragma endregion ���̑��̐ݒ�
    //----------------

    depth.SetDepthStencilState( pipeline_.pipelineDesc_);

    //���[�g�V�O�l�`���̐���--------------------------
#pragma region ���[�g�V�O�l�`���̐���


     rootsignature_.InitRootSignatureDesc(rootparams,  texSample_);

     rootsignature_.SetSerializeRootSignature( shader_);

     rootsignature_.CreateRootSignature();

    // �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g

     pipeline_.SetRootSignature( rootsignature_);

#pragma endregion ���[�g�V�O�l�`���̐���
    //--------------------------------

   //�p�C�v���C���X�e�[�g�̐���-------------------------
#pragma region �p�C�v���C���X�e�[�g�̐���

     pipeline_.CreateGraphicsPipelineState();

#pragma endregion �p�C�v���C���X�e�[�g�̐���
    //-----------------------------



}

void MCB::PipelineRootSignature::CreateLinePipeline(Depth& depth, RootParameter& rootparams, uint16_t blendMode)
{

    texSample_.Init();

    // ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��--------------------------------
#pragma region ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��

    shader_.ShaderCompile(L"Resources\\Shaders\\LineSimpleShader\\LineVertexShader.hlsl", "main", VS);

#pragma endregion ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //------------------------------------------

     //�W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��---------------
#pragma region �W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��

    //shader_.ShaderCompile(L"Resources\\Shaders\\LineSimpleShader\\LineGeometryShader.hlsl", "main", GS);

#pragma endregion �W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //---------------------------------


    // �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��-------------------------------
#pragma region �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��

    shader_.ShaderCompile(L"Resources\\Shaders\\LineSimpleShader\\LinePixelShader.hlsl", "main", PS);

#pragma endregion �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //--------------------------------

#pragma region ���_�V�F�[�_�ƃs�N�Z���V�F�[�_���p�C�v���C���ɐݒ�

    pipeline_.SetGpipleneDescAll(&shader_);

#pragma endregion ���_�V�F�[�_�ƃs�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
    //-----------------------------------

    //�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�------------------------------------
#pragma region �T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
    pipeline_.SetSampleMask();

    pipeline_.SetAllAddRasterizerState();
#pragma endregion �T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
    //------------------------------------


     //�u�����h�X�e�[�g�̐ݒ�-------------------------------
#pragma region �u�����h�X�e�[�g�̐ݒ�

    pipeline_.SetRenderTaegetBlendDesc(pipeline_.pipelineDesc_.BlendState.RenderTarget[0]);

    pipeline_.SetRenderTargetWriteMask();

    pipeline_.SetNormalBlendDesc();
    switch (blendMode)
    {
    case Alpha:
        pipeline_.SetAlphaBlend();
        break;
    case Add:
        pipeline_.SetAddBlend();
        break;
    case Sub:
        pipeline_.SetSubBlend();
        break;
    case Inv:
        pipeline_.SetInvBlend();
        break;
    default:
        pipeline_.SetAlphaBlend();
        break;
    }


#pragma endregion �u�����h�X�e�[�g�̐ݒ�
    //--------------------------

    //���_���C�A�E�g�̐ݒ�------------------
#pragma region ���_���C�A�E�g�̐ݒ�

    pipeline_.pipelineDesc_.InputLayout.pInputElementDescs = shader_.inputLayout_;
    pipeline_.pipelineDesc_.InputLayout.NumElements = _countof(shader_.inputLayout_);

#pragma endregion ���_���C�A�E�g�̐ݒ�
    //----------------------------

    //�}�`�̌`����O�p�`�ɐݒ�-------------------------
    pipeline_.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE);
    //------------------

    //���̑��̐ݒ�----------------
#pragma region ���̑��̐ݒ�


    pipeline_.SetNumRenderTargets(2);
    pipeline_.SetRTVFormats(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 0);
    pipeline_.SetRTVFormats(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 1);
    pipeline_.SetSampleDescCount();

#pragma endregion ���̑��̐ݒ�
    //----------------

    depth.SetDepthStencilState(pipeline_.pipelineDesc_);

    //���[�g�V�O�l�`���̐���--------------------------
#pragma region ���[�g�V�O�l�`���̐���


    rootsignature_.InitRootSignatureDesc(rootparams, texSample_);

    rootsignature_.SetSerializeRootSignature(shader_);

    rootsignature_.CreateRootSignature();

    // �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g

    pipeline_.SetRootSignature(rootsignature_);

#pragma endregion ���[�g�V�O�l�`���̐���
    //--------------------------------

   //�p�C�v���C���X�e�[�g�̐���-------------------------
#pragma region �p�C�v���C���X�e�[�g�̐���

    pipeline_.CreateGraphicsPipelineState();

#pragma endregion �p�C�v���C���X�e�[�g�̐���
    //-----------------------------



}

void MCB::PipelineRootSignature::Create3DObjectWirePipeline(Depth& depth, RootParameter& rootparam, uint16_t blendMode)
{

    texSample_.Init();

    // ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��--------------------------------
#pragma region ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��

    shader_.ShaderCompile(L"Resources\\Shaders\\ObjSimpleShader\\OBJSimpleVertexShader.hlsl", "main", VS);

#pragma endregion ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //------------------------------------------

     //�W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��---------------
#pragma region �W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��

    shader_.ShaderCompile(L"Resources\\Shaders\\ObjSimpleShader\\OBJSimpleGeometryShader.hlsl", "main", GS);

#pragma endregion �W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //---------------------------------


    // �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��-------------------------------
#pragma region �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��

    shader_.ShaderCompile(L"Resources\\Shaders\\ObjSimpleShader\\OBJSimplePixelShader.hlsl", "main", PS);

#pragma endregion �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //--------------------------------

#pragma region ���_�V�F�[�_�ƃs�N�Z���V�F�[�_���p�C�v���C���ɐݒ�

    pipeline_.SetGpipleneDescAll(&shader_);

#pragma endregion ���_�V�F�[�_�ƃs�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
    //-----------------------------------

    //�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�------------------------------------
#pragma region �T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
    pipeline_.SetSampleMask();

    pipeline_.SetRasterizerState(true,D3D12_CULL_MODE_BACK, D3D12_FILL_MODE_WIREFRAME);
#pragma endregion �T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
    //------------------------------------


     //�u�����h�X�e�[�g�̐ݒ�-------------------------------
#pragma region �u�����h�X�e�[�g�̐ݒ�

    pipeline_.SetRenderTaegetBlendDesc(pipeline_.pipelineDesc_.BlendState.RenderTarget[0]);

    pipeline_.SetRenderTargetWriteMask();

    pipeline_.SetNormalBlendDesc();
    switch (blendMode)
    {
    case Alpha:
        pipeline_.SetAlphaBlend();
        break;
    case Add:
        pipeline_.SetAddBlend();
        break;
    case Sub:
        pipeline_.SetSubBlend();
        break;
    case Inv:
        pipeline_.SetInvBlend();
        break;
    default:
        pipeline_.SetAlphaBlend();
        break;
    }


#pragma endregion �u�����h�X�e�[�g�̐ݒ�
    //--------------------------

    //���_���C�A�E�g�̐ݒ�------------------
#pragma region ���_���C�A�E�g�̐ݒ�

    pipeline_.pipelineDesc_.InputLayout.pInputElementDescs = shader_.inputLayout_;
    pipeline_.pipelineDesc_.InputLayout.NumElements = _countof(shader_.inputLayout_);

#pragma endregion ���_���C�A�E�g�̐ݒ�
    //----------------------------

    //�}�`�̌`����O�p�`�ɐݒ�-------------------------
    pipeline_.SetPrimitiveTopologyType();
    //------------------

    //���̑��̐ݒ�----------------
#pragma region ���̑��̐ݒ�


    pipeline_.SetNumRenderTargets(2);
    pipeline_.SetRTVFormats(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 0);
    pipeline_.SetRTVFormats(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 1);
    pipeline_.SetSampleDescCount();

#pragma endregion ���̑��̐ݒ�
    //----------------

    depth.SetDepthStencilState(pipeline_.pipelineDesc_);

    //���[�g�V�O�l�`���̐���--------------------------
#pragma region ���[�g�V�O�l�`���̐���


    rootsignature_.InitRootSignatureDesc(rootparam, texSample_);

    rootsignature_.SetSerializeRootSignature(shader_);

    rootsignature_.CreateRootSignature();

    // �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g

    pipeline_.SetRootSignature(rootsignature_);

#pragma endregion ���[�g�V�O�l�`���̐���
    //--------------------------------

   //�p�C�v���C���X�e�[�g�̐���-------------------------
#pragma region �p�C�v���C���X�e�[�g�̐���

    pipeline_.CreateGraphicsPipelineState();

#pragma endregion �p�C�v���C���X�e�[�g�̐���
    //-----------------------------


}

void MCB::PipelineRootSignature::CreateSpritePipeline(Depth& depth, RootParameter& rootparams, uint16_t blendMode)
{



     texSample_.Init();

    // ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��--------------------------------
#pragma region ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��

     shader_.ShaderCompile(L"Resources\\Shaders\\SpriteShader\\SpriteVertexShader.hlsl", "main", VS);

#pragma endregion ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //------------------------------------------

    // �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��-------------------------------
#pragma region �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��

     shader_.ShaderCompile(L"Resources\\Shaders\\SpriteShader\\SpritePixelShader.hlsl", "main", PS);

#pragma endregion �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //--------------------------------

#pragma region ���_�V�F�[�_�ƃs�N�Z���V�F�[�_���p�C�v���C���ɐݒ�

     pipeline_.SetSpriteGpipleneDescAll(& shader_);

#pragma endregion ���_�V�F�[�_�ƃs�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
    //-----------------------------------

    //�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�------------------------------------
#pragma region �T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
     pipeline_.SetSampleMask();

     pipeline_.SetSpriteAllAddRasterizerState();

#pragma endregion �T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
    //------------------------------------


     //�u�����h�X�e�[�g�̐ݒ�-------------------------------
#pragma region �u�����h�X�e�[�g�̐ݒ�

     pipeline_.SetRenderTaegetBlendDesc( pipeline_.pipelineDesc_.BlendState.RenderTarget[0]);

     pipeline_.SetRenderTargetWriteMask();

     pipeline_.SetNormalBlendDesc();

    switch (blendMode)
    {
    case Alpha:
         pipeline_.SetAlphaBlend();
        break;
    case Add:
         pipeline_.SetAddBlend();
        break;
    case Sub:
         pipeline_.SetSubBlend();
        break;
    case Inv:
         pipeline_.SetInvBlend();
        break;
    default:
         pipeline_.SetAlphaBlend();
        break;
    }

#pragma endregion �u�����h�X�e�[�g�̐ݒ�
    //--------------------------

    //���_���C�A�E�g�̐ݒ�------------------
#pragma region ���_���C�A�E�g�̐ݒ�

     pipeline_.pipelineDesc_.InputLayout.pInputElementDescs =  shader_.SpriteinputLayout_;
     pipeline_.pipelineDesc_.InputLayout.NumElements = _countof( shader_.SpriteinputLayout_);

#pragma endregion ���_���C�A�E�g�̐ݒ�
    //----------------------------

    //�}�`�̌`����O�p�`�ɐݒ�-------------------------
     pipeline_.SetPrimitiveTopologyType();
    //------------------

    //���̑��̐ݒ�----------------
#pragma region ���̑��̐ݒ�

     pipeline_.SetNumRenderTargets();
     pipeline_.SetRTVFormats();
     pipeline_.SetSampleDescCount();

#pragma endregion ���̑��̐ݒ�
    //----------------

    depth.SetSpriteDepthStencilState( pipeline_.pipelineDesc_);

    //���[�g�V�O�l�`���̐���--------------------------
#pragma region ���[�g�V�O�l�`���̐���


     rootsignature_.InitRootSignatureDesc(rootparams,  texSample_);

     rootsignature_.SetSerializeRootSignature( shader_);

     rootsignature_.CreateRootSignature();

    // �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g

     pipeline_.SetRootSignature( rootsignature_);

#pragma endregion ���[�g�V�O�l�`���̐���
    //--------------------------------

   //�p�C�v���C���X�e�[�g�̐���-------------------------
#pragma region �p�C�v���C���X�e�[�g�̐���

     pipeline_.CreateGraphicsPipelineState();

#pragma endregion �p�C�v���C���X�e�[�g�̐���
    //-----------------------------


}

void MCB::PipelineRootSignature::CreateParticlePipeline(Depth& depth, RootParameter& rootparams, uint16_t blendMode)
{


     texSample_.Init();

    // ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��--------------------------------
#pragma region ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��

     shader_.ShaderCompile(L"Resources\\Shaders\\ParticleShader\\ParticleVertexShader.hlsl", "main", VS);

#pragma endregion ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //------------------------------------------

     //�W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��---------------
#pragma region �W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��

     shader_.ShaderCompile(L"Resources\\Shaders\\ParticleShader\\ParticleGeometryShader.hlsl", "main", GS);

#pragma endregion �W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //---------------------------------


    // �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��-------------------------------
#pragma region �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��

     shader_.ShaderCompile(L"Resources\\Shaders\\ParticleShader\\ParticlePixelShader.hlsl", "main", PS);

#pragma endregion �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //--------------------------------

#pragma region ���_�V�F�[�_�ƃs�N�Z���V�F�[�_���p�C�v���C���ɐݒ�

     pipeline_.SetGpipleneDescAll(& shader_);

#pragma endregion ���_�V�F�[�_�ƃs�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
    //-----------------------------------

    //�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�------------------------------------
#pragma region �T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
     pipeline_.SetSampleMask();

     pipeline_.SetAllAddRasterizerState();
#pragma endregion �T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
    //------------------------------------


     //�u�����h�X�e�[�g�̐ݒ�-------------------------------
#pragma region �u�����h�X�e�[�g�̐ݒ�

     pipeline_.SetRenderTaegetBlendDesc( pipeline_.pipelineDesc_.BlendState.RenderTarget[0]);

     pipeline_.SetRenderTargetWriteMask();

     pipeline_.SetNormalBlendDesc();
    switch (blendMode)
    {
    case Alpha:
         pipeline_.SetAlphaBlend();
        break;
    case Add:
         pipeline_.SetAddBlend();
        break;
    case Sub:
         pipeline_.SetSubBlend();
        break;
    case Inv:
         pipeline_.SetInvBlend();
        break;
    default:
         pipeline_.SetAlphaBlend();
        break;
    }
#pragma endregion �u�����h�X�e�[�g�̐ݒ�
    //--------------------------

    //���_���C�A�E�g�̐ݒ�------------------
#pragma region ���_���C�A�E�g�̐ݒ�

     pipeline_.pipelineDesc_.InputLayout.pInputElementDescs =  shader_.inputLayout_;
     pipeline_.pipelineDesc_.InputLayout.NumElements = _countof( shader_.inputLayout_);

#pragma endregion ���_���C�A�E�g�̐ݒ�
    //----------------------------

    //�}�`�̌`����O�p�`�ɐݒ�-------------------------
     pipeline_.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT);
    //------------------

    //���̑��̐ݒ�----------------
#pragma region ���̑��̐ݒ�

     pipeline_.SetNumRenderTargets();
     pipeline_.SetRTVFormats();
     pipeline_.SetSampleDescCount();

#pragma endregion ���̑��̐ݒ�
    //----------------

    depth.SetParticleDepthStencilState( pipeline_.pipelineDesc_);

    //���[�g�V�O�l�`���̐���--------------------------
#pragma region ���[�g�V�O�l�`���̐���


     rootsignature_.InitRootSignatureDesc(rootparams,  texSample_);

     rootsignature_.SetSerializeRootSignature( shader_);

     rootsignature_.CreateRootSignature();

    // �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g

     pipeline_.SetRootSignature( rootsignature_);

#pragma endregion ���[�g�V�O�l�`���̐���
    //--------------------------------

   //�p�C�v���C���X�e�[�g�̐���-------------------------
#pragma region �p�C�v���C���X�e�[�g�̐���

     pipeline_.CreateGraphicsPipelineState();

#pragma endregion �p�C�v���C���X�e�[�g�̐���
    //-----------------------------



}

void MCB::PipelineRootSignature::CreateFbxPipeline(Depth& depth, RootParameter& rootparam, uint16_t blendMode)
{


    texSample_.Init();

    // ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��--------------------------------
#pragma region ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��

    shader_.ShaderCompile(L"Resources\\Shaders\\FbxShader\\FbxVertexShader.hlsl", "main", VS);

#pragma endregion ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //------------------------------------------

     //�W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��---------------
#pragma region �W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��

    shader_.ShaderCompile(L"Resources\\Shaders\\FbxShader\\FbxGeometryShader.hlsl", "main", GS);

#pragma endregion �W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //---------------------------------


    // �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��-------------------------------
#pragma region �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��

    shader_.ShaderCompile(L"Resources\\Shaders\\FbxShader\\FbxPixelShader.hlsl", "main", PS);

#pragma endregion �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //--------------------------------

#pragma region ���_�V�F�[�_�ƃs�N�Z���V�F�[�_���p�C�v���C���ɐݒ�

    pipeline_.SetGpipleneDescAll(&shader_);

#pragma endregion ���_�V�F�[�_�ƃs�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
    //-----------------------------------

    //�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�------------------------------------
#pragma region �T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
    pipeline_.SetSampleMask();

    pipeline_.SetAllAddRasterizerState();
#pragma endregion �T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
    //------------------------------------


     //�u�����h�X�e�[�g�̐ݒ�-------------------------------
#pragma region �u�����h�X�e�[�g�̐ݒ�

    pipeline_.SetRenderTaegetBlendDesc(pipeline_.pipelineDesc_.BlendState.RenderTarget[0]);

    pipeline_.SetRenderTargetWriteMask();

    pipeline_.SetNormalBlendDesc();
    switch (blendMode)
    {
    case Alpha:
        pipeline_.SetAlphaBlend();
        break;
    case Add:
        pipeline_.SetAddBlend();
        break;
    case Sub:
        pipeline_.SetSubBlend();
        break;
    case Inv:
        pipeline_.SetInvBlend();
        break;
    default:
        pipeline_.SetAlphaBlend();
        break;
    }


#pragma endregion �u�����h�X�e�[�g�̐ݒ�
    //--------------------------

    //���_���C�A�E�g�̐ݒ�------------------
#pragma region ���_���C�A�E�g�̐ݒ�

    pipeline_.pipelineDesc_.InputLayout.pInputElementDescs = shader_.fbxinputLayout_;
    pipeline_.pipelineDesc_.InputLayout.NumElements = _countof(shader_.fbxinputLayout_);

#pragma endregion ���_���C�A�E�g�̐ݒ�
    //----------------------------

    //�}�`�̌`����O�p�`�ɐݒ�-------------------------
    pipeline_.SetPrimitiveTopologyType();
    //------------------

    //���̑��̐ݒ�----------------
#pragma region ���̑��̐ݒ�


    pipeline_.SetNumRenderTargets(2);
    pipeline_.SetRTVFormats(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 0);
    pipeline_.SetRTVFormats(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 1);
    pipeline_.SetSampleDescCount();

#pragma endregion ���̑��̐ݒ�
    //----------------

    depth.SetDepthStencilState(pipeline_.pipelineDesc_);

    //���[�g�V�O�l�`���̐���--------------------------
#pragma region ���[�g�V�O�l�`���̐���


    rootsignature_.InitRootSignatureDesc(rootparam, texSample_);

    rootsignature_.SetSerializeRootSignature(shader_);

    rootsignature_.CreateRootSignature();

    // �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g

    pipeline_.SetRootSignature(rootsignature_);

#pragma endregion ���[�g�V�O�l�`���̐���
    //--------------------------------

   //�p�C�v���C���X�e�[�g�̐���-------------------------
#pragma region �p�C�v���C���X�e�[�g�̐���

    pipeline_.CreateGraphicsPipelineState();

#pragma endregion �p�C�v���C���X�e�[�g�̐���
    //-----------------------------


}

void MCB::PipelineRootSignature::CreateFbxWirePipeline(Depth& depth, RootParameter& rootparam, uint16_t blendMode)
{


    texSample_.Init();

    // ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��--------------------------------
#pragma region ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��

    shader_.ShaderCompile(L"Resources\\Shaders\\FbxSimpleShader\\FbxSimpleVertexShader.hlsl", "main", VS);

#pragma endregion ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //------------------------------------------

     //�W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��---------------
#pragma region �W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��

    shader_.ShaderCompile(L"Resources\\Shaders\\FbxSimpleShader\\FbxSimpleGeometryShader.hlsl", "main", GS);

#pragma endregion �W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //---------------------------------


    // �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��-------------------------------
#pragma region �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��

    shader_.ShaderCompile(L"Resources\\Shaders\\FbxSimpleShader\\FbxSimplePixelShader.hlsl", "main", PS);

#pragma endregion �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //--------------------------------

#pragma region ���_�V�F�[�_�ƃs�N�Z���V�F�[�_���p�C�v���C���ɐݒ�

    pipeline_.SetGpipleneDescAll(&shader_);

#pragma endregion ���_�V�F�[�_�ƃs�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
    //-----------------------------------

    //�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�------------------------------------
#pragma region �T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
    pipeline_.SetSampleMask();

    pipeline_.SetRasterizerState(true, D3D12_CULL_MODE_BACK, D3D12_FILL_MODE_WIREFRAME);
#pragma endregion �T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
    //------------------------------------


     //�u�����h�X�e�[�g�̐ݒ�-------------------------------
#pragma region �u�����h�X�e�[�g�̐ݒ�

    pipeline_.SetRenderTaegetBlendDesc(pipeline_.pipelineDesc_.BlendState.RenderTarget[0]);

    pipeline_.SetRenderTargetWriteMask();

    pipeline_.SetNormalBlendDesc();
    switch (blendMode)
    {
    case Alpha:
        pipeline_.SetAlphaBlend();
        break;
    case Add:
        pipeline_.SetAddBlend();
        break;
    case Sub:
        pipeline_.SetSubBlend();
        break;
    case Inv:
        pipeline_.SetInvBlend();
        break;
    default:
        pipeline_.SetAlphaBlend();
        break;
    }


#pragma endregion �u�����h�X�e�[�g�̐ݒ�
    //--------------------------

    //���_���C�A�E�g�̐ݒ�------------------
#pragma region ���_���C�A�E�g�̐ݒ�

    pipeline_.pipelineDesc_.InputLayout.pInputElementDescs = shader_.fbxinputLayout_;
    pipeline_.pipelineDesc_.InputLayout.NumElements = _countof(shader_.fbxinputLayout_);

#pragma endregion ���_���C�A�E�g�̐ݒ�
    //----------------------------

    //�}�`�̌`����O�p�`�ɐݒ�-------------------------
    pipeline_.SetPrimitiveTopologyType();
    //------------------

    //���̑��̐ݒ�----------------
#pragma region ���̑��̐ݒ�


    pipeline_.SetNumRenderTargets(2);
    pipeline_.SetRTVFormats(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 0);
    pipeline_.SetRTVFormats(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 1);
    pipeline_.SetSampleDescCount();

#pragma endregion ���̑��̐ݒ�
    //----------------

    depth.SetDepthStencilState(pipeline_.pipelineDesc_);

    //���[�g�V�O�l�`���̐���--------------------------
#pragma region ���[�g�V�O�l�`���̐���


    rootsignature_.InitRootSignatureDesc(rootparam, texSample_);

    rootsignature_.SetSerializeRootSignature(shader_);

    rootsignature_.CreateRootSignature();

    // �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g

    pipeline_.SetRootSignature(rootsignature_);

#pragma endregion ���[�g�V�O�l�`���̐���
    //--------------------------------

   //�p�C�v���C���X�e�[�g�̐���-------------------------
#pragma region �p�C�v���C���X�e�[�g�̐���

    pipeline_.CreateGraphicsPipelineState();

#pragma endregion �p�C�v���C���X�e�[�g�̐���
    //-----------------------------


}

void MCB::PipelineRootSignature::CreatePostEffectPipeline(Depth& depth, RootParameter& rootparam, uint16_t blendMode)
{



    texSample_.Init();
    texSample_.samplerDesc_.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
    texSample_.samplerDesc_.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
    // ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��--------------------------------
#pragma region ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��

    shader_.ShaderCompile(L"Resources\\Shaders\\PostEffectShader\\PostEffectVertexShader.hlsl", "main", VS);

#pragma endregion ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //------------------------------------------


    // �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��-------------------------------
#pragma region �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��

    shader_.ShaderCompile(L"Resources\\Shaders\\PostEffectShader\\PostEffectPixelShader.hlsl", "main", PS);

#pragma endregion �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //--------------------------------

#pragma region ���_�V�F�[�_�ƃs�N�Z���V�F�[�_���p�C�v���C���ɐݒ�

    pipeline_.SetSpriteGpipleneDescAll(&shader_);

#pragma endregion ���_�V�F�[�_�ƃs�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
    //-----------------------------------

    //�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�------------------------------------
#pragma region �T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
    pipeline_.SetSampleMask();

    pipeline_.SetSpriteAllAddRasterizerState();

#pragma endregion �T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
    //------------------------------------


     //�u�����h�X�e�[�g�̐ݒ�-------------------------------
#pragma region �u�����h�X�e�[�g�̐ݒ�

    pipeline_.SetRenderTaegetBlendDesc(pipeline_.pipelineDesc_.BlendState.RenderTarget[0]);

    pipeline_.SetRenderTargetWriteMask();

    pipeline_.SetNormalBlendDesc();

    switch (blendMode)
    {
    case Alpha:
        pipeline_.SetAlphaBlend();
        break;
    case Add:
        pipeline_.SetAddBlend();
        break;
    case Sub:
        pipeline_.SetSubBlend();
        break;
    case Inv:
        pipeline_.SetInvBlend();
        break;
    default:
        pipeline_.SetAlphaBlend();
        break;
    }

#pragma endregion �u�����h�X�e�[�g�̐ݒ�
    //--------------------------

    //���_���C�A�E�g�̐ݒ�------------------
#pragma region ���_���C�A�E�g�̐ݒ�

    pipeline_.pipelineDesc_.InputLayout.pInputElementDescs = shader_.SpriteinputLayout_;
    pipeline_.pipelineDesc_.InputLayout.NumElements = _countof(shader_.SpriteinputLayout_);

#pragma endregion ���_���C�A�E�g�̐ݒ�
    //----------------------------

    //�}�`�̌`����O�p�`�ɐݒ�-------------------------
    pipeline_.SetPrimitiveTopologyType();
    //------------------

    //���̑��̐ݒ�----------------
#pragma region ���̑��̐ݒ�

    pipeline_.SetNumRenderTargets();
    pipeline_.SetRTVFormats();
    pipeline_.SetSampleDescCount();

#pragma endregion ���̑��̐ݒ�
    //----------------

    depth.SetSpriteDepthStencilState(pipeline_.pipelineDesc_);

    //���[�g�V�O�l�`���̐���--------------------------
#pragma region ���[�g�V�O�l�`���̐���


    rootsignature_.InitRootSignatureDesc(rootparam, texSample_);

    rootsignature_.SetSerializeRootSignature(shader_);

    rootsignature_.CreateRootSignature();

    // �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g

    pipeline_.SetRootSignature(rootsignature_);

#pragma endregion ���[�g�V�O�l�`���̐���
    //--------------------------------

   //�p�C�v���C���X�e�[�g�̐���-------------------------
#pragma region �p�C�v���C���X�e�[�g�̐���

    pipeline_.CreateGraphicsPipelineState();

#pragma endregion �p�C�v���C���X�e�[�g�̐���
    //-----------------------------


}

void MCB::PipelineRootSignature::SetBrendMode(BlendMode blendMode)
{
    switch (blendMode)
    {
    case Alpha:
        pipeline_.SetAlphaBlend();
        break;
    case Add:
        pipeline_.SetAddBlend();
        break;
    case Sub:
        pipeline_.SetSubBlend();
        break;
    case Inv:
        pipeline_.SetInvBlend();
        break;
    default:
        pipeline_.SetAlphaBlend();
        break;
    }
}

void MCB::PipelineRootSignature::CommonBeginDraw(Toporogy toporogyType)
{
    Dx12* dx12 = Dx12::GetInstance();
    dx12->commandList_->SetPipelineState(pipeline_.pipelinestate_.Get());
    dx12->commandList_->SetGraphicsRootSignature(rootsignature_.rootsignature_.Get());

    switch (toporogyType)
    {
    case TRIANGLELIST:
        dx12->commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        break;
    case TRIANGLESTRIP:
        dx12->commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
        break;
    case POINT:
        dx12->commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
        break;
    case LINE:
        dx12->commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
        break;
    default:
        break;
    }
    //�v���~�e�B�u�`��̐ݒ�R�}���h�i�O�p�`���X�g�j--------------------------

    //SRV�q�[�v�̐ݒ�R�}���h
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> ppHeaps[] = { ShaderResource::GetInstance()->srvHeap_ };
    dx12->commandList_->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps->GetAddressOf());
}
