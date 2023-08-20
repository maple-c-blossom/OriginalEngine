#include "PIpelineRootSignature.h"
using namespace MCB;

void MCB::PipelineRootSignature::Create3DObjectPipeline( Depth& depth, RootParameter& rootparams, uint16_t blendMode)
{

     texSample_.Init();

    // 頂点シェーダの読み込みとコンパイル--------------------------------
#pragma region 頂点シェーダの読み込みとコンパイル

     shader_.ShaderCompile(L"Resources\\Shaders\\OBJShader\\OBJVertexShader.hlsl", "main", VS);

#pragma endregion 頂点シェーダの読み込みとコンパイル
    //------------------------------------------

     //ジオメトリシェーダの読み込みとコンパイル---------------
#pragma region ジオメトリシェーダの読み込みとコンパイル

     shader_.ShaderCompile(L"Resources\\Shaders\\OBJShader\\OBJGeometryShader.hlsl", "main", GS);

#pragma endregion ジオメトリシェーダの読み込みとコンパイル
    //---------------------------------


    // ピクセルシェーダの読み込みとコンパイル-------------------------------
#pragma region ピクセルシェーダの読み込みとコンパイル

     shader_.ShaderCompile(L"Resources\\Shaders\\OBJShader\\OBJPixelShader.hlsl", "main", PS);

#pragma endregion ピクセルシェーダの読み込みとコンパイル
    //--------------------------------

#pragma region 頂点シェーダとピクセルシェーダをパイプラインに設定

     pipeline_.SetGpipleneDescAll(& shader_);

#pragma endregion 頂点シェーダとピクセルシェーダをパイプラインに設定
    //-----------------------------------

    //サンプルマスクとラスタライザステートの設定------------------------------------
#pragma region サンプルマスクとラスタライザステートの設定
     pipeline_.SetSampleMask();

     pipeline_.SetAllAddRasterizerState();
#pragma endregion サンプルマスクとラスタライザステートの設定
    //------------------------------------


     //ブレンドステートの設定-------------------------------
#pragma region ブレンドステートの設定

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


#pragma endregion ブレンドステートの設定
    //--------------------------

    //頂点レイアウトの設定------------------
#pragma region 頂点レイアウトの設定

     pipeline_.pipelineDesc_.InputLayout.pInputElementDescs =   shader_.inputLayout_;
     pipeline_.pipelineDesc_.InputLayout.NumElements = _countof( shader_.inputLayout_);

#pragma endregion 頂点レイアウトの設定
    //----------------------------

    //図形の形状を三角形に設定-------------------------
     pipeline_.SetPrimitiveTopologyType();
    //------------------

    //その他の設定----------------
#pragma region その他の設定


     pipeline_.SetNumRenderTargets(2);
     pipeline_.SetRTVFormats(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 0);
     pipeline_.SetRTVFormats(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 1);
     pipeline_.SetSampleDescCount();

#pragma endregion その他の設定
    //----------------

    depth.SetDepthStencilState( pipeline_.pipelineDesc_);

    //ルートシグネチャの生成--------------------------
#pragma region ルートシグネチャの生成


     rootsignature_.InitRootSignatureDesc(rootparams,  texSample_);

     rootsignature_.SetSerializeRootSignature( shader_);

     rootsignature_.CreateRootSignature();

    // パイプラインにルートシグネチャをセット

     pipeline_.SetRootSignature( rootsignature_);

#pragma endregion ルートシグネチャの生成
    //--------------------------------

   //パイプラインステートの生成-------------------------
#pragma region パイプラインステートの生成

     pipeline_.CreateGraphicsPipelineState();

#pragma endregion パイプラインステートの生成
    //-----------------------------



}

void MCB::PipelineRootSignature::CreateLinePipeline(Depth& depth, RootParameter& rootparams, uint16_t blendMode)
{

    texSample_.Init();

    // 頂点シェーダの読み込みとコンパイル--------------------------------
#pragma region 頂点シェーダの読み込みとコンパイル

    shader_.ShaderCompile(L"Resources\\Shaders\\LineSimpleShader\\LineVertexShader.hlsl", "main", VS);

#pragma endregion 頂点シェーダの読み込みとコンパイル
    //------------------------------------------

     //ジオメトリシェーダの読み込みとコンパイル---------------
#pragma region ジオメトリシェーダの読み込みとコンパイル

    //shader_.ShaderCompile(L"Resources\\Shaders\\LineSimpleShader\\LineGeometryShader.hlsl", "main", GS);

#pragma endregion ジオメトリシェーダの読み込みとコンパイル
    //---------------------------------


    // ピクセルシェーダの読み込みとコンパイル-------------------------------
#pragma region ピクセルシェーダの読み込みとコンパイル

    shader_.ShaderCompile(L"Resources\\Shaders\\LineSimpleShader\\LinePixelShader.hlsl", "main", PS);

#pragma endregion ピクセルシェーダの読み込みとコンパイル
    //--------------------------------

#pragma region 頂点シェーダとピクセルシェーダをパイプラインに設定

    pipeline_.SetGpipleneDescAll(&shader_);

#pragma endregion 頂点シェーダとピクセルシェーダをパイプラインに設定
    //-----------------------------------

    //サンプルマスクとラスタライザステートの設定------------------------------------
#pragma region サンプルマスクとラスタライザステートの設定
    pipeline_.SetSampleMask();

    pipeline_.SetAllAddRasterizerState();
#pragma endregion サンプルマスクとラスタライザステートの設定
    //------------------------------------


     //ブレンドステートの設定-------------------------------
#pragma region ブレンドステートの設定

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


#pragma endregion ブレンドステートの設定
    //--------------------------

    //頂点レイアウトの設定------------------
#pragma region 頂点レイアウトの設定

    pipeline_.pipelineDesc_.InputLayout.pInputElementDescs = shader_.inputLayout_;
    pipeline_.pipelineDesc_.InputLayout.NumElements = _countof(shader_.inputLayout_);

#pragma endregion 頂点レイアウトの設定
    //----------------------------

    //図形の形状を三角形に設定-------------------------
    pipeline_.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE);
    //------------------

    //その他の設定----------------
#pragma region その他の設定


    pipeline_.SetNumRenderTargets(2);
    pipeline_.SetRTVFormats(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 0);
    pipeline_.SetRTVFormats(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 1);
    pipeline_.SetSampleDescCount();

#pragma endregion その他の設定
    //----------------

    depth.SetDepthStencilState(pipeline_.pipelineDesc_);

    //ルートシグネチャの生成--------------------------
#pragma region ルートシグネチャの生成


    rootsignature_.InitRootSignatureDesc(rootparams, texSample_);

    rootsignature_.SetSerializeRootSignature(shader_);

    rootsignature_.CreateRootSignature();

    // パイプラインにルートシグネチャをセット

    pipeline_.SetRootSignature(rootsignature_);

#pragma endregion ルートシグネチャの生成
    //--------------------------------

   //パイプラインステートの生成-------------------------
#pragma region パイプラインステートの生成

    pipeline_.CreateGraphicsPipelineState();

#pragma endregion パイプラインステートの生成
    //-----------------------------



}

void MCB::PipelineRootSignature::Create3DObjectWirePipeline(Depth& depth, RootParameter& rootparam, uint16_t blendMode)
{

    texSample_.Init();

    // 頂点シェーダの読み込みとコンパイル--------------------------------
#pragma region 頂点シェーダの読み込みとコンパイル

    shader_.ShaderCompile(L"Resources\\Shaders\\ObjSimpleShader\\OBJSimpleVertexShader.hlsl", "main", VS);

#pragma endregion 頂点シェーダの読み込みとコンパイル
    //------------------------------------------

     //ジオメトリシェーダの読み込みとコンパイル---------------
#pragma region ジオメトリシェーダの読み込みとコンパイル

    shader_.ShaderCompile(L"Resources\\Shaders\\ObjSimpleShader\\OBJSimpleGeometryShader.hlsl", "main", GS);

#pragma endregion ジオメトリシェーダの読み込みとコンパイル
    //---------------------------------


    // ピクセルシェーダの読み込みとコンパイル-------------------------------
#pragma region ピクセルシェーダの読み込みとコンパイル

    shader_.ShaderCompile(L"Resources\\Shaders\\ObjSimpleShader\\OBJSimplePixelShader.hlsl", "main", PS);

#pragma endregion ピクセルシェーダの読み込みとコンパイル
    //--------------------------------

#pragma region 頂点シェーダとピクセルシェーダをパイプラインに設定

    pipeline_.SetGpipleneDescAll(&shader_);

#pragma endregion 頂点シェーダとピクセルシェーダをパイプラインに設定
    //-----------------------------------

    //サンプルマスクとラスタライザステートの設定------------------------------------
#pragma region サンプルマスクとラスタライザステートの設定
    pipeline_.SetSampleMask();

    pipeline_.SetRasterizerState(true,D3D12_CULL_MODE_BACK, D3D12_FILL_MODE_WIREFRAME);
#pragma endregion サンプルマスクとラスタライザステートの設定
    //------------------------------------


     //ブレンドステートの設定-------------------------------
#pragma region ブレンドステートの設定

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


#pragma endregion ブレンドステートの設定
    //--------------------------

    //頂点レイアウトの設定------------------
#pragma region 頂点レイアウトの設定

    pipeline_.pipelineDesc_.InputLayout.pInputElementDescs = shader_.inputLayout_;
    pipeline_.pipelineDesc_.InputLayout.NumElements = _countof(shader_.inputLayout_);

#pragma endregion 頂点レイアウトの設定
    //----------------------------

    //図形の形状を三角形に設定-------------------------
    pipeline_.SetPrimitiveTopologyType();
    //------------------

    //その他の設定----------------
#pragma region その他の設定


    pipeline_.SetNumRenderTargets(2);
    pipeline_.SetRTVFormats(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 0);
    pipeline_.SetRTVFormats(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 1);
    pipeline_.SetSampleDescCount();

#pragma endregion その他の設定
    //----------------

    depth.SetDepthStencilState(pipeline_.pipelineDesc_);

    //ルートシグネチャの生成--------------------------
#pragma region ルートシグネチャの生成


    rootsignature_.InitRootSignatureDesc(rootparam, texSample_);

    rootsignature_.SetSerializeRootSignature(shader_);

    rootsignature_.CreateRootSignature();

    // パイプラインにルートシグネチャをセット

    pipeline_.SetRootSignature(rootsignature_);

#pragma endregion ルートシグネチャの生成
    //--------------------------------

   //パイプラインステートの生成-------------------------
#pragma region パイプラインステートの生成

    pipeline_.CreateGraphicsPipelineState();

#pragma endregion パイプラインステートの生成
    //-----------------------------


}

void MCB::PipelineRootSignature::CreateSpritePipeline(Depth& depth, RootParameter& rootparams, uint16_t blendMode)
{



     texSample_.Init();

    // 頂点シェーダの読み込みとコンパイル--------------------------------
#pragma region 頂点シェーダの読み込みとコンパイル

     shader_.ShaderCompile(L"Resources\\Shaders\\SpriteShader\\SpriteVertexShader.hlsl", "main", VS);

#pragma endregion 頂点シェーダの読み込みとコンパイル
    //------------------------------------------

    // ピクセルシェーダの読み込みとコンパイル-------------------------------
#pragma region ピクセルシェーダの読み込みとコンパイル

     shader_.ShaderCompile(L"Resources\\Shaders\\SpriteShader\\SpritePixelShader.hlsl", "main", PS);

#pragma endregion ピクセルシェーダの読み込みとコンパイル
    //--------------------------------

#pragma region 頂点シェーダとピクセルシェーダをパイプラインに設定

     pipeline_.SetSpriteGpipleneDescAll(& shader_);

#pragma endregion 頂点シェーダとピクセルシェーダをパイプラインに設定
    //-----------------------------------

    //サンプルマスクとラスタライザステートの設定------------------------------------
#pragma region サンプルマスクとラスタライザステートの設定
     pipeline_.SetSampleMask();

     pipeline_.SetSpriteAllAddRasterizerState();

#pragma endregion サンプルマスクとラスタライザステートの設定
    //------------------------------------


     //ブレンドステートの設定-------------------------------
#pragma region ブレンドステートの設定

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

#pragma endregion ブレンドステートの設定
    //--------------------------

    //頂点レイアウトの設定------------------
#pragma region 頂点レイアウトの設定

     pipeline_.pipelineDesc_.InputLayout.pInputElementDescs =  shader_.SpriteinputLayout_;
     pipeline_.pipelineDesc_.InputLayout.NumElements = _countof( shader_.SpriteinputLayout_);

#pragma endregion 頂点レイアウトの設定
    //----------------------------

    //図形の形状を三角形に設定-------------------------
     pipeline_.SetPrimitiveTopologyType();
    //------------------

    //その他の設定----------------
#pragma region その他の設定

     pipeline_.SetNumRenderTargets();
     pipeline_.SetRTVFormats();
     pipeline_.SetSampleDescCount();

#pragma endregion その他の設定
    //----------------

    depth.SetSpriteDepthStencilState( pipeline_.pipelineDesc_);

    //ルートシグネチャの生成--------------------------
#pragma region ルートシグネチャの生成


     rootsignature_.InitRootSignatureDesc(rootparams,  texSample_);

     rootsignature_.SetSerializeRootSignature( shader_);

     rootsignature_.CreateRootSignature();

    // パイプラインにルートシグネチャをセット

     pipeline_.SetRootSignature( rootsignature_);

#pragma endregion ルートシグネチャの生成
    //--------------------------------

   //パイプラインステートの生成-------------------------
#pragma region パイプラインステートの生成

     pipeline_.CreateGraphicsPipelineState();

#pragma endregion パイプラインステートの生成
    //-----------------------------


}

void MCB::PipelineRootSignature::CreateParticlePipeline(Depth& depth, RootParameter& rootparams, uint16_t blendMode)
{


     texSample_.Init();

    // 頂点シェーダの読み込みとコンパイル--------------------------------
#pragma region 頂点シェーダの読み込みとコンパイル

     shader_.ShaderCompile(L"Resources\\Shaders\\ParticleShader\\ParticleVertexShader.hlsl", "main", VS);

#pragma endregion 頂点シェーダの読み込みとコンパイル
    //------------------------------------------

     //ジオメトリシェーダの読み込みとコンパイル---------------
#pragma region ジオメトリシェーダの読み込みとコンパイル

     shader_.ShaderCompile(L"Resources\\Shaders\\ParticleShader\\ParticleGeometryShader.hlsl", "main", GS);

#pragma endregion ジオメトリシェーダの読み込みとコンパイル
    //---------------------------------


    // ピクセルシェーダの読み込みとコンパイル-------------------------------
#pragma region ピクセルシェーダの読み込みとコンパイル

     shader_.ShaderCompile(L"Resources\\Shaders\\ParticleShader\\ParticlePixelShader.hlsl", "main", PS);

#pragma endregion ピクセルシェーダの読み込みとコンパイル
    //--------------------------------

#pragma region 頂点シェーダとピクセルシェーダをパイプラインに設定

     pipeline_.SetGpipleneDescAll(& shader_);

#pragma endregion 頂点シェーダとピクセルシェーダをパイプラインに設定
    //-----------------------------------

    //サンプルマスクとラスタライザステートの設定------------------------------------
#pragma region サンプルマスクとラスタライザステートの設定
     pipeline_.SetSampleMask();

     pipeline_.SetAllAddRasterizerState();
#pragma endregion サンプルマスクとラスタライザステートの設定
    //------------------------------------


     //ブレンドステートの設定-------------------------------
#pragma region ブレンドステートの設定

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
#pragma endregion ブレンドステートの設定
    //--------------------------

    //頂点レイアウトの設定------------------
#pragma region 頂点レイアウトの設定

     pipeline_.pipelineDesc_.InputLayout.pInputElementDescs =  shader_.inputLayout_;
     pipeline_.pipelineDesc_.InputLayout.NumElements = _countof( shader_.inputLayout_);

#pragma endregion 頂点レイアウトの設定
    //----------------------------

    //図形の形状を三角形に設定-------------------------
     pipeline_.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT);
    //------------------

    //その他の設定----------------
#pragma region その他の設定

     pipeline_.SetNumRenderTargets();
     pipeline_.SetRTVFormats();
     pipeline_.SetSampleDescCount();

#pragma endregion その他の設定
    //----------------

    depth.SetParticleDepthStencilState( pipeline_.pipelineDesc_);

    //ルートシグネチャの生成--------------------------
#pragma region ルートシグネチャの生成


     rootsignature_.InitRootSignatureDesc(rootparams,  texSample_);

     rootsignature_.SetSerializeRootSignature( shader_);

     rootsignature_.CreateRootSignature();

    // パイプラインにルートシグネチャをセット

     pipeline_.SetRootSignature( rootsignature_);

#pragma endregion ルートシグネチャの生成
    //--------------------------------

   //パイプラインステートの生成-------------------------
#pragma region パイプラインステートの生成

     pipeline_.CreateGraphicsPipelineState();

#pragma endregion パイプラインステートの生成
    //-----------------------------



}

void MCB::PipelineRootSignature::CreateFbxPipeline(Depth& depth, RootParameter& rootparam, uint16_t blendMode)
{


    texSample_.Init();

    // 頂点シェーダの読み込みとコンパイル--------------------------------
#pragma region 頂点シェーダの読み込みとコンパイル

    shader_.ShaderCompile(L"Resources\\Shaders\\FbxShader\\FbxVertexShader.hlsl", "main", VS);

#pragma endregion 頂点シェーダの読み込みとコンパイル
    //------------------------------------------

     //ジオメトリシェーダの読み込みとコンパイル---------------
#pragma region ジオメトリシェーダの読み込みとコンパイル

    shader_.ShaderCompile(L"Resources\\Shaders\\FbxShader\\FbxGeometryShader.hlsl", "main", GS);

#pragma endregion ジオメトリシェーダの読み込みとコンパイル
    //---------------------------------


    // ピクセルシェーダの読み込みとコンパイル-------------------------------
#pragma region ピクセルシェーダの読み込みとコンパイル

    shader_.ShaderCompile(L"Resources\\Shaders\\FbxShader\\FbxPixelShader.hlsl", "main", PS);

#pragma endregion ピクセルシェーダの読み込みとコンパイル
    //--------------------------------

#pragma region 頂点シェーダとピクセルシェーダをパイプラインに設定

    pipeline_.SetGpipleneDescAll(&shader_);

#pragma endregion 頂点シェーダとピクセルシェーダをパイプラインに設定
    //-----------------------------------

    //サンプルマスクとラスタライザステートの設定------------------------------------
#pragma region サンプルマスクとラスタライザステートの設定
    pipeline_.SetSampleMask();

    pipeline_.SetAllAddRasterizerState();
#pragma endregion サンプルマスクとラスタライザステートの設定
    //------------------------------------


     //ブレンドステートの設定-------------------------------
#pragma region ブレンドステートの設定

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


#pragma endregion ブレンドステートの設定
    //--------------------------

    //頂点レイアウトの設定------------------
#pragma region 頂点レイアウトの設定

    pipeline_.pipelineDesc_.InputLayout.pInputElementDescs = shader_.fbxinputLayout_;
    pipeline_.pipelineDesc_.InputLayout.NumElements = _countof(shader_.fbxinputLayout_);

#pragma endregion 頂点レイアウトの設定
    //----------------------------

    //図形の形状を三角形に設定-------------------------
    pipeline_.SetPrimitiveTopologyType();
    //------------------

    //その他の設定----------------
#pragma region その他の設定


    pipeline_.SetNumRenderTargets(2);
    pipeline_.SetRTVFormats(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 0);
    pipeline_.SetRTVFormats(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 1);
    pipeline_.SetSampleDescCount();

#pragma endregion その他の設定
    //----------------

    depth.SetDepthStencilState(pipeline_.pipelineDesc_);

    //ルートシグネチャの生成--------------------------
#pragma region ルートシグネチャの生成


    rootsignature_.InitRootSignatureDesc(rootparam, texSample_);

    rootsignature_.SetSerializeRootSignature(shader_);

    rootsignature_.CreateRootSignature();

    // パイプラインにルートシグネチャをセット

    pipeline_.SetRootSignature(rootsignature_);

#pragma endregion ルートシグネチャの生成
    //--------------------------------

   //パイプラインステートの生成-------------------------
#pragma region パイプラインステートの生成

    pipeline_.CreateGraphicsPipelineState();

#pragma endregion パイプラインステートの生成
    //-----------------------------


}

void MCB::PipelineRootSignature::CreateFbxWirePipeline(Depth& depth, RootParameter& rootparam, uint16_t blendMode)
{


    texSample_.Init();

    // 頂点シェーダの読み込みとコンパイル--------------------------------
#pragma region 頂点シェーダの読み込みとコンパイル

    shader_.ShaderCompile(L"Resources\\Shaders\\FbxSimpleShader\\FbxSimpleVertexShader.hlsl", "main", VS);

#pragma endregion 頂点シェーダの読み込みとコンパイル
    //------------------------------------------

     //ジオメトリシェーダの読み込みとコンパイル---------------
#pragma region ジオメトリシェーダの読み込みとコンパイル

    shader_.ShaderCompile(L"Resources\\Shaders\\FbxSimpleShader\\FbxSimpleGeometryShader.hlsl", "main", GS);

#pragma endregion ジオメトリシェーダの読み込みとコンパイル
    //---------------------------------


    // ピクセルシェーダの読み込みとコンパイル-------------------------------
#pragma region ピクセルシェーダの読み込みとコンパイル

    shader_.ShaderCompile(L"Resources\\Shaders\\FbxSimpleShader\\FbxSimplePixelShader.hlsl", "main", PS);

#pragma endregion ピクセルシェーダの読み込みとコンパイル
    //--------------------------------

#pragma region 頂点シェーダとピクセルシェーダをパイプラインに設定

    pipeline_.SetGpipleneDescAll(&shader_);

#pragma endregion 頂点シェーダとピクセルシェーダをパイプラインに設定
    //-----------------------------------

    //サンプルマスクとラスタライザステートの設定------------------------------------
#pragma region サンプルマスクとラスタライザステートの設定
    pipeline_.SetSampleMask();

    pipeline_.SetRasterizerState(true, D3D12_CULL_MODE_BACK, D3D12_FILL_MODE_WIREFRAME);
#pragma endregion サンプルマスクとラスタライザステートの設定
    //------------------------------------


     //ブレンドステートの設定-------------------------------
#pragma region ブレンドステートの設定

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


#pragma endregion ブレンドステートの設定
    //--------------------------

    //頂点レイアウトの設定------------------
#pragma region 頂点レイアウトの設定

    pipeline_.pipelineDesc_.InputLayout.pInputElementDescs = shader_.fbxinputLayout_;
    pipeline_.pipelineDesc_.InputLayout.NumElements = _countof(shader_.fbxinputLayout_);

#pragma endregion 頂点レイアウトの設定
    //----------------------------

    //図形の形状を三角形に設定-------------------------
    pipeline_.SetPrimitiveTopologyType();
    //------------------

    //その他の設定----------------
#pragma region その他の設定


    pipeline_.SetNumRenderTargets(2);
    pipeline_.SetRTVFormats(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 0);
    pipeline_.SetRTVFormats(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 1);
    pipeline_.SetSampleDescCount();

#pragma endregion その他の設定
    //----------------

    depth.SetDepthStencilState(pipeline_.pipelineDesc_);

    //ルートシグネチャの生成--------------------------
#pragma region ルートシグネチャの生成


    rootsignature_.InitRootSignatureDesc(rootparam, texSample_);

    rootsignature_.SetSerializeRootSignature(shader_);

    rootsignature_.CreateRootSignature();

    // パイプラインにルートシグネチャをセット

    pipeline_.SetRootSignature(rootsignature_);

#pragma endregion ルートシグネチャの生成
    //--------------------------------

   //パイプラインステートの生成-------------------------
#pragma region パイプラインステートの生成

    pipeline_.CreateGraphicsPipelineState();

#pragma endregion パイプラインステートの生成
    //-----------------------------


}

void MCB::PipelineRootSignature::CreatePostEffectPipeline(Depth& depth, RootParameter& rootparam, uint16_t blendMode)
{



    texSample_.Init();
    texSample_.samplerDesc_.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
    texSample_.samplerDesc_.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
    // 頂点シェーダの読み込みとコンパイル--------------------------------
#pragma region 頂点シェーダの読み込みとコンパイル

    shader_.ShaderCompile(L"Resources\\Shaders\\PostEffectShader\\PostEffectVertexShader.hlsl", "main", VS);

#pragma endregion 頂点シェーダの読み込みとコンパイル
    //------------------------------------------


    // ピクセルシェーダの読み込みとコンパイル-------------------------------
#pragma region ピクセルシェーダの読み込みとコンパイル

    shader_.ShaderCompile(L"Resources\\Shaders\\PostEffectShader\\PostEffectPixelShader.hlsl", "main", PS);

#pragma endregion ピクセルシェーダの読み込みとコンパイル
    //--------------------------------

#pragma region 頂点シェーダとピクセルシェーダをパイプラインに設定

    pipeline_.SetSpriteGpipleneDescAll(&shader_);

#pragma endregion 頂点シェーダとピクセルシェーダをパイプラインに設定
    //-----------------------------------

    //サンプルマスクとラスタライザステートの設定------------------------------------
#pragma region サンプルマスクとラスタライザステートの設定
    pipeline_.SetSampleMask();

    pipeline_.SetSpriteAllAddRasterizerState();

#pragma endregion サンプルマスクとラスタライザステートの設定
    //------------------------------------


     //ブレンドステートの設定-------------------------------
#pragma region ブレンドステートの設定

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

#pragma endregion ブレンドステートの設定
    //--------------------------

    //頂点レイアウトの設定------------------
#pragma region 頂点レイアウトの設定

    pipeline_.pipelineDesc_.InputLayout.pInputElementDescs = shader_.SpriteinputLayout_;
    pipeline_.pipelineDesc_.InputLayout.NumElements = _countof(shader_.SpriteinputLayout_);

#pragma endregion 頂点レイアウトの設定
    //----------------------------

    //図形の形状を三角形に設定-------------------------
    pipeline_.SetPrimitiveTopologyType();
    //------------------

    //その他の設定----------------
#pragma region その他の設定

    pipeline_.SetNumRenderTargets();
    pipeline_.SetRTVFormats();
    pipeline_.SetSampleDescCount();

#pragma endregion その他の設定
    //----------------

    depth.SetSpriteDepthStencilState(pipeline_.pipelineDesc_);

    //ルートシグネチャの生成--------------------------
#pragma region ルートシグネチャの生成


    rootsignature_.InitRootSignatureDesc(rootparam, texSample_);

    rootsignature_.SetSerializeRootSignature(shader_);

    rootsignature_.CreateRootSignature();

    // パイプラインにルートシグネチャをセット

    pipeline_.SetRootSignature(rootsignature_);

#pragma endregion ルートシグネチャの生成
    //--------------------------------

   //パイプラインステートの生成-------------------------
#pragma region パイプラインステートの生成

    pipeline_.CreateGraphicsPipelineState();

#pragma endregion パイプラインステートの生成
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
    //プリミティブ形状の設定コマンド（三角形リスト）--------------------------

    //SRVヒープの設定コマンド
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> ppHeaps[] = { ShaderResource::GetInstance()->srvHeap_ };
    dx12->commandList_->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps->GetAddressOf());
}
