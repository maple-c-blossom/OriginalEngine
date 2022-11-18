#include "PIpelineRootSignature.h"
using namespace MCB;

void MCB::PipelineRootSignature::Create3DObjectPipeline( Depth& depth, RootParameter& rootparams,int blendMode)
{

     texSample.Init();

    // 頂点シェーダの読み込みとコンパイル--------------------------------
#pragma region 頂点シェーダの読み込みとコンパイル

     shader.ShaderCompile(L"Resources\\Shaders\\OBJShader\\OBJVertexShader.hlsl", "main", VS);

#pragma endregion 頂点シェーダの読み込みとコンパイル
    //------------------------------------------

     //ジオメトリシェーダの読み込みとコンパイル---------------
#pragma region ジオメトリシェーダの読み込みとコンパイル

     shader.ShaderCompile(L"Resources\\Shaders\\OBJShader\\OBJGeometryShader.hlsl", "main", GS);

#pragma endregion ジオメトリシェーダの読み込みとコンパイル
    //---------------------------------


    // ピクセルシェーダの読み込みとコンパイル-------------------------------
#pragma region ピクセルシェーダの読み込みとコンパイル

     shader.ShaderCompile(L"Resources\\Shaders\\OBJShader\\OBJPixelShader.hlsl", "main", PS);

#pragma endregion ピクセルシェーダの読み込みとコンパイル
    //--------------------------------

#pragma region 頂点シェーダとピクセルシェーダをパイプラインに設定

     pipeline.SetGpipleneDescAll(& shader);

#pragma endregion 頂点シェーダとピクセルシェーダをパイプラインに設定
    //-----------------------------------

    //サンプルマスクとラスタライザステートの設定------------------------------------
#pragma region サンプルマスクとラスタライザステートの設定
     pipeline.SetSampleMask();

     pipeline.SetAllAddRasterizerState();
#pragma endregion サンプルマスクとラスタライザステートの設定
    //------------------------------------


     //ブレンドステートの設定-------------------------------
#pragma region ブレンドステートの設定

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


#pragma endregion ブレンドステートの設定
    //--------------------------

    //頂点レイアウトの設定------------------
#pragma region 頂点レイアウトの設定

     pipeline.pipelineDesc.InputLayout.pInputElementDescs =   shader.inputLayout;
     pipeline.pipelineDesc.InputLayout.NumElements = _countof( shader.inputLayout);

#pragma endregion 頂点レイアウトの設定
    //----------------------------

    //図形の形状を三角形に設定-------------------------
     pipeline.SetPrimitiveTopologyType();
    //------------------

    //その他の設定----------------
#pragma region その他の設定

     pipeline.SetNumRenderTargets();
     pipeline.SetRTVFormats();
     pipeline.SetSampleDescCount();

#pragma endregion その他の設定
    //----------------

    depth.SetDepthStencilState( pipeline.pipelineDesc);

    //ルートシグネチャの生成--------------------------
#pragma region ルートシグネチャの生成


     rootsignature.InitRootSignatureDesc(rootparams,  texSample);

     rootsignature.SetSerializeRootSignature( shader);

     rootsignature.CreateRootSignature();

    // パイプラインにルートシグネチャをセット

     pipeline.SetRootSignature( rootsignature);

#pragma endregion ルートシグネチャの生成
    //--------------------------------

   //パイプラインステートの生成-------------------------
#pragma region パイプラインステートの生成

     pipeline.CreateGraphicsPipelineState();

#pragma endregion パイプラインステートの生成
    //-----------------------------



}

void MCB::PipelineRootSignature::CreateSpritePipeline(Depth& depth, RootParameter& rootparams, int blendMode)
{



     texSample.Init();

    // 頂点シェーダの読み込みとコンパイル--------------------------------
#pragma region 頂点シェーダの読み込みとコンパイル

     shader.ShaderCompile(L"Resources\\Shaders\\SpriteShader\\SpriteVertexShader.hlsl", "main", VS);

#pragma endregion 頂点シェーダの読み込みとコンパイル
    //------------------------------------------

    // ピクセルシェーダの読み込みとコンパイル-------------------------------
#pragma region ピクセルシェーダの読み込みとコンパイル

     shader.ShaderCompile(L"Resources\\Shaders\\SpriteShader\\SpritePixelShader.hlsl", "main", PS);

#pragma endregion ピクセルシェーダの読み込みとコンパイル
    //--------------------------------

#pragma region 頂点シェーダとピクセルシェーダをパイプラインに設定

     pipeline.SetSpriteGpipleneDescAll(& shader);

#pragma endregion 頂点シェーダとピクセルシェーダをパイプラインに設定
    //-----------------------------------

    //サンプルマスクとラスタライザステートの設定------------------------------------
#pragma region サンプルマスクとラスタライザステートの設定
     pipeline.SetSampleMask();

     pipeline.SetSpriteAllAddRasterizerState();

#pragma endregion サンプルマスクとラスタライザステートの設定
    //------------------------------------


     //ブレンドステートの設定-------------------------------
#pragma region ブレンドステートの設定

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

#pragma endregion ブレンドステートの設定
    //--------------------------

    //頂点レイアウトの設定------------------
#pragma region 頂点レイアウトの設定

     pipeline.pipelineDesc.InputLayout.pInputElementDescs =  shader.SpriteinputLayout;
     pipeline.pipelineDesc.InputLayout.NumElements = _countof( shader.SpriteinputLayout);

#pragma endregion 頂点レイアウトの設定
    //----------------------------

    //図形の形状を三角形に設定-------------------------
     pipeline.SetPrimitiveTopologyType();
    //------------------

    //その他の設定----------------
#pragma region その他の設定

     pipeline.SetNumRenderTargets();
     pipeline.SetRTVFormats();
     pipeline.SetSampleDescCount();

#pragma endregion その他の設定
    //----------------

    depth.SetSpriteDepthStencilState( pipeline.pipelineDesc);

    //ルートシグネチャの生成--------------------------
#pragma region ルートシグネチャの生成


     rootsignature.InitRootSignatureDesc(rootparams,  texSample);

     rootsignature.SetSerializeRootSignature( shader);

     rootsignature.CreateRootSignature();

    // パイプラインにルートシグネチャをセット

     pipeline.SetRootSignature( rootsignature);

#pragma endregion ルートシグネチャの生成
    //--------------------------------

   //パイプラインステートの生成-------------------------
#pragma region パイプラインステートの生成

     pipeline.CreateGraphicsPipelineState();

#pragma endregion パイプラインステートの生成
    //-----------------------------


}

void MCB::PipelineRootSignature::CreateParticlePipeline(Depth& depth, RootParameter& rootparams, int blendMode)
{


     texSample.Init();

    // 頂点シェーダの読み込みとコンパイル--------------------------------
#pragma region 頂点シェーダの読み込みとコンパイル

     shader.ShaderCompile(L"Resources\\Shaders\\ParticleShader\\ParticleVertexShader.hlsl", "main", VS);

#pragma endregion 頂点シェーダの読み込みとコンパイル
    //------------------------------------------

     //ジオメトリシェーダの読み込みとコンパイル---------------
#pragma region ジオメトリシェーダの読み込みとコンパイル

     shader.ShaderCompile(L"Resources\\Shaders\\ParticleShader\\ParticleGeometryShader.hlsl", "main", GS);

#pragma endregion ジオメトリシェーダの読み込みとコンパイル
    //---------------------------------


    // ピクセルシェーダの読み込みとコンパイル-------------------------------
#pragma region ピクセルシェーダの読み込みとコンパイル

     shader.ShaderCompile(L"Resources\\Shaders\\ParticleShader\\ParticlePixelShader.hlsl", "main", PS);

#pragma endregion ピクセルシェーダの読み込みとコンパイル
    //--------------------------------

#pragma region 頂点シェーダとピクセルシェーダをパイプラインに設定

     pipeline.SetGpipleneDescAll(& shader);

#pragma endregion 頂点シェーダとピクセルシェーダをパイプラインに設定
    //-----------------------------------

    //サンプルマスクとラスタライザステートの設定------------------------------------
#pragma region サンプルマスクとラスタライザステートの設定
     pipeline.SetSampleMask();

     pipeline.SetAllAddRasterizerState();
#pragma endregion サンプルマスクとラスタライザステートの設定
    //------------------------------------


     //ブレンドステートの設定-------------------------------
#pragma region ブレンドステートの設定

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
#pragma endregion ブレンドステートの設定
    //--------------------------

    //頂点レイアウトの設定------------------
#pragma region 頂点レイアウトの設定

     pipeline.pipelineDesc.InputLayout.pInputElementDescs =  shader.inputLayout;
     pipeline.pipelineDesc.InputLayout.NumElements = _countof( shader.inputLayout);

#pragma endregion 頂点レイアウトの設定
    //----------------------------

    //図形の形状を三角形に設定-------------------------
     pipeline.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT);
    //------------------

    //その他の設定----------------
#pragma region その他の設定

     pipeline.SetNumRenderTargets();
     pipeline.SetRTVFormats();
     pipeline.SetSampleDescCount();

#pragma endregion その他の設定
    //----------------

    depth.SetParticleDepthStencilState( pipeline.pipelineDesc);

    //ルートシグネチャの生成--------------------------
#pragma region ルートシグネチャの生成


     rootsignature.InitRootSignatureDesc(rootparams,  texSample);

     rootsignature.SetSerializeRootSignature( shader);

     rootsignature.CreateRootSignature();

    // パイプラインにルートシグネチャをセット

     pipeline.SetRootSignature( rootsignature);

#pragma endregion ルートシグネチャの生成
    //--------------------------------

   //パイプラインステートの生成-------------------------
#pragma region パイプラインステートの生成

     pipeline.CreateGraphicsPipelineState();

#pragma endregion パイプラインステートの生成
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
    //プリミティブ形状の設定コマンド（三角形リスト）--------------------------

    //SRVヒープの設定コマンド
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> ppHeaps[] = { ShaderResource::GetInstance()->srvHeap };
    dx12->commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps->GetAddressOf());
}
