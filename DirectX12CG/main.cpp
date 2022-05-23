#define _CRTDBG_MAP_ALLOC 
#include <cstdlib>
#include <crtdbg.h>

#pragma region 標準.h include

#include <d3dcompiler.h>
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <string>
#include <DirectXMath.h>
#include <wrl.h>
#include <cassert>
#include <memory>
#include <DirectXTex.h>

#pragma endregion 標準.h include

#pragma region 自作.h include

#include "Input.h"
#include "DxWindow.h"
#include "Dx12.h"
#include "View.h"
#include "Projection.h"
#include "WorldMatrix.h"
#include "Depth.h"
#include "Object3d.h"
#include "ObjectMaterial.h"
#include "TextureFile.h"
#include "MipMap.h"
#include "TexImgData.h"
#include "TextureBuffer.h"
#include "Descriptor.h"
#include "RootParameter.h"
#include "Vertex.h"
#include "MCBMatrix.h"
#include "Util.h"
#include "Shader.h"
#include "Pipeline.h"
#include "TexSample.h"
#include "RootSignature.h"

#pragma endregion 自作.h include

#pragma region pragma comment

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#pragma endregion pragma comment

#pragma region using namespace

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace MCB;

#pragma endregion using namespace

// Windowsアプリでのエントリーポイント(main関数) 
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow)
{
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(434);

    //int* hoge = new int(4);
    DxWindow* dxWindow = new DxWindow;

#pragma region DirectX初期化
    //デバック時のみ----------
#pragma region デバック時のみ

#ifdef _DEBUG
//デバックレイヤーをオンに
    ComPtr<ID3D12Debug> debugController;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
    {
        debugController->EnableDebugLayer();
    }

#endif

#pragma endregion デバック時のみ
    //-------------

    //DirectXクラス生成
    Dx12* dx = new Dx12(*dxWindow);
    //inputクラス生成
    Input* input = new Input(dx->result, dxWindow->window, dxWindow->hwnd);

#pragma endregion 


#pragma region 描画初期化処理

    //深度バッファ----
    Depth depth(*dxWindow, *dx);
    //-------

    ////3Dオブジェクトマテリアルの生成-------------------
    //ObjectMaterial objMaterial;
    //objMaterial.Init(*dx);
    ////---------------------

    //3Dオブジェクトの生成-------------------
#pragma region 3Dオブジェクトの生成
    Object3d triangle(*dx);
    triangle.Init(*dx);
    triangle.model.CreateModel("triangle");
    triangle.scale = { 20,20,20 };
#pragma endregion 3Dオブジェクトの生成
    //----------------------

     //行列-----------------------
#pragma region 行列
        //ビュー変換行列
        View matView;
        matView.CreateMatrixView(XMFLOAT3(0.0f, 0.0f, -100.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
        //射影変換行列
        Projection matProjection;
         matProjection.CreateMatrixProjection(XMConvertToRadians(45.0f),(float)dxWindow->window_width / dxWindow->window_height, 0.1f, 1000.0f);
#pragma endregion 行列
     //---------------------

#pragma region 画像関係
     //画像ファイル--------------------
     //TextureFile* textureFile = new TextureFile;
     //dx->result = textureFile->LoadTexture(L"Resources\\tori.png", WIC_FLAGS_NONE);
     //----------------------------

     //ミップマップの生成-------------------------
     MipMap* mipmap = new MipMap;
     dx->result = mipmap->GenerateMipMap(&triangle.model.texture, TEX_FILTER_DEFAULT, 0);
     //----------------------------

     //画像イメージデータの作成----------------------
     TexImgData* imageData = new TexImgData;
     imageData->SetImageDataRGBA(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
     //------------------------------------

      //テクスチャバッファ設定---------------------------------------
      TextureBuffer texBuff;
      texBuff.SetTexHeapProp(D3D12_HEAP_TYPE_CUSTOM,D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,D3D12_MEMORY_POOL_L0);
      texBuff.SetTexResourceDesc(triangle.model.texture, D3D12_RESOURCE_DIMENSION_TEXTURE2D, 1);
      //--------------------------------------


      //テクスチャバッファの生成----------------------
      dx->result = texBuff.CommitResouce(*dx, D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr);
      texBuff.TransferMipmatToTexBuff(triangle.model.texture, nullptr, dx->result);
      //-----------------------------------
#pragma endregion 画像関係

         //デスクリプタヒープの生成-------------------------
#pragma region デスクリプタヒープの生成

     const size_t kMaxSRVCount = 2056;
     Descriptor descriptor;
     descriptor.SetHeapDesc(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, kMaxSRVCount);
     dx->result = descriptor.SetDescriptorHeap(*dx);
     descriptor.SetSrvHeap();

#pragma endregion デスクリプタヒープの生成
    //-------------------------------

     //シェーダーリソースビューの作成------------------------------
#pragma region シェーダーリソースビューの作成

     descriptor.SetSrvDesc(texBuff, D3D12_SRV_DIMENSION_TEXTURE2D);

     descriptor.SetShaderResourceView(*dx, texBuff);

#pragma endregion シェーダーリソースビューの作成
     //----------------------------

     //デスクリプタレンジの設定--------------------------------
#pragma region デスクリプタレンジの設定

     descriptor.SetDescriptorRange(1, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0);

#pragma endregion デスクリプタレンジの設定
     //-----------------------------------------

     //ルートパラメータの設定---------------------------
#pragma region ルートパラメータの設定

     RootParameter rootparams;
     rootparams.SetRootParam(D3D12_ROOT_PARAMETER_TYPE_CBV, 0, 0, D3D12_SHADER_VISIBILITY_ALL,descriptor,0);
     rootparams.SetRootParam(D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE, 0, 0, D3D12_SHADER_VISIBILITY_ALL,descriptor,1);
     rootparams.SetRootParam(D3D12_ROOT_PARAMETER_TYPE_CBV, 1, 0, D3D12_SHADER_VISIBILITY_ALL, descriptor, 0);
#pragma endregion ルートパラメータの設定
     //------------------------

     //頂点データ---------------------------------
#pragma region 頂点データ
   
#pragma endregion 頂点データ
     //--------------------------
    
     //インデックスバッファの設定-------------------------
#pragma region インデックスの設定
     triangle.model.SetSizeIB();

     triangle.model.material.SetIndex(D3D12_RESOURCE_DIMENSION_BUFFER, triangle.model.sizeIB, 1, 1, 1, 1, D3D12_TEXTURE_LAYOUT_ROW_MAJOR);

#pragma endregion インデックスの設定
     //------------------------

#pragma region インデックスバッファ生成

     triangle.model.CreateIndexBuffer(*dx, triangle.model.material.HeapProp, D3D12_HEAP_FLAG_NONE, triangle.model.material.Resdesc, D3D12_RESOURCE_STATE_GENERIC_READ);

#pragma endregion インデックスバッファ生成

     //インデックスバッファへのデータ転送------------------------------
#pragma region インデックスバッファへのデータ転送

     dx->result = triangle.model.IndexMaping();

#pragma endregion インデックスバッファへのデータ転送
    //-------------------------------------

     //インデックスバッファビューの作成-----------------------------------
#pragma region インデックスバッファビューの作成
     triangle.model.SetIbView(DXGI_FORMAT_R16_UINT);
#pragma endregion インデックスバッファビューの作成
     //------------------------------------------

     //頂点バッファ---------------
#pragma region 頂点バッファの設定
     triangle.model.SetSizeVB();
     triangle.model.material.SetVertexBuffer(D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_DIMENSION_BUFFER, triangle.model.sizeVB, 1, 1, 1, 1, D3D12_TEXTURE_LAYOUT_ROW_MAJOR);
#pragma endregion 頂点バッファの設定
     //----------------------------------

     // 頂点バッファの生成----------------------------
#pragma region 頂点バッファの生成

     triangle.model.CreateVertexBuffer(*dx, triangle.model.material.HeapProp, D3D12_HEAP_FLAG_NONE, triangle.model.material.Resdesc, D3D12_RESOURCE_STATE_GENERIC_READ);

#pragma endregion 頂点バッファの生成
     //-------------------------

     //法線ベクトル計算---------------------------
#pragma region 法線ベクトル計算
     //triangle.vertex.CalculationNormalVec();
#pragma endregion 法線ベクトルを計算
     //-------------------------

     // 頂点バッファへのデータ転送------------
#pragma region 頂点バッファへのデータ転送
     triangle.model.VertexMaping();
#pragma endregion 頂点バッファへのデータ転送
     //--------------------------------------

     // 頂点バッファビューの作成--------------------------
#pragma region 頂点バッファビューの作成
     triangle.model.SetVbView();
#pragma endregion 頂点バッファビューの作成
     //-----------------------------------

    //シェーダーオブジェクト宣言-------------------------------------------
#pragma region シェーダーオブジェクト宣言
     Shader shader;
#pragma endregion シェーダーオブジェクト宣言
    //---------------------------------

    // 頂点シェーダの読み込みとコンパイル--------------------------------
#pragma region 頂点シェーダの読み込みとコンパイル

     shader.ShaderCompile(L"OBJVertexShader.hlsl", "main", VS);

#pragma endregion 頂点シェーダの読み込みとコンパイル
    //------------------------------------------

     // ピクセルシェーダの読み込みとコンパイル-------------------------------
#pragma region ピクセルシェーダの読み込みとコンパイル
     
     shader.ShaderCompile(L"OBJPixelShader.hlsl", "main", PS);

#pragma endregion ピクセルシェーダの読み込みとコンパイル
     //--------------------------------

     Pipeline pipleline;

     //頂点シェーダ、ピクセルシェーダをパイプラインに設定-----------------------------
#pragma region 頂点シェーダとピクセルシェーダをパイプラインに設定

     pipleline.SetGpipleneDescAll(&shader);

#pragma endregion 頂点シェーダとピクセルシェーダをパイプラインに設定
     //-----------------------------------

     //サンプルマスクとラスタライザステートの設定------------------------------------
#pragma region サンプルマスクとラスタライザステートの設定
     pipleline.SetSampleMask();

     pipleline.SetAllAddRasterizerState();
#pragma endregion サンプルマスクとラスタライザステートの設定
     //------------------------------------


      //ブレンドステートの設定-------------------------------
#pragma region ブレンドステートの設定

     pipleline.SetRenderTaegetBlendDesc(pipleline.pipelineDesc.BlendState.RenderTarget[0]);

     pipleline.SetRenderTargetWriteMask();

     pipleline.SetNormalBlendDesc();

     pipleline.SetAlphaBlend();


#pragma endregion ブレンドステートの設定
     //--------------------------

     //頂点レイアウトの設定------------------
#pragma region 頂点レイアウトの設定

     pipleline.pipelineDesc.InputLayout.pInputElementDescs = shader.inputLayout;
     pipleline.pipelineDesc.InputLayout.NumElements = _countof(shader.inputLayout);

#pragma endregion 頂点レイアウトの設定
     //----------------------------

     //図形の形状を三角形に設定-------------------------
     pipleline.SetPrimitiveTopologyType();
     //------------------

     //その他の設定----------------
#pragma region その他の設定

     pipleline.SetNumRenderTargets();
     pipleline.SetRTVFormats();
     pipleline.SetSampleDescCount();

#pragma endregion その他の設定
    //----------------

     depth.SetDepthStencilState(pipleline.pipelineDesc);

   //テクスチャサンプラーの設定-----------------------
#pragma region テクスチャサンプラーの設定

     TexSample sample;
     sample.Init();

#pragma endregion テクスチャサンプラーの設定
   //----------------------------------

     //ルートシグネチャの生成--------------------------
#pragma region ルートシグネチャの生成

     RootSignature rootsignature;

     rootsignature.InitRootSignatureDesc(rootparams, sample);

     rootsignature.SetSerializeRootSignature(shader,*dx);

     rootsignature.CreateRootSignature(dx);

     // パイプラインにルートシグネチャをセット

     pipleline.SetRootSignature(rootsignature);

#pragma endregion ルートシグネチャの生成
     //--------------------------------

    //パイプラインステートの生成-------------------------
#pragma region パイプラインステートの生成

     pipleline.CreateGraphicsPipelineState(dx);

#pragma endregion パイプラインステートの生成
     //-----------------------------

     float clearColor[] = { 0.1f,0.25f, 0.5f,0.0f }; // 青っぽい色

#pragma endregion

     //ゲームループ用変数--------------------------------
#pragma region ゲームループ用変数
     float angle = 0.0f;
     const int DEFAULT_MOVE_LIMIT = 60;

     XMFLOAT3 targetVec = { 0,0,1 };
     XMFLOAT3 Angle = { 0,0,0 };


#pragma endregion ゲームループ用変数
     //--------------------------
     
     //ゲームループ-------------------------------------
#pragma region ゲームループ
    while (true)
    {
        input->UpDateInit(dx->result);

        dxWindow->messageUpdate();

        if (input->IsKeyDown(DIK_ESCAPE) || dxWindow->breakFlag)
        {
            break;
        }

#pragma region 更新処理
        if (input->IsKeyDown(DIK_RIGHT) || input->IsKeyDown(DIK_LEFT) || input->IsKeyDown(DIK_UP) || input->IsKeyDown(DIK_DOWN))
        {

            if (input->IsKeyDown(DIK_RIGHT)) { Angle.y += 0.05f; };
            if (input->IsKeyDown(DIK_LEFT)) { Angle.y -= 0.05f; };

            if (input->IsKeyDown(DIK_UP)) { Angle.x += 0.05f; };
            if (input->IsKeyDown(DIK_DOWN)) { Angle.x -= 0.05f; };

            targetVec.x = sinf(Angle.y);
            targetVec.y = sinf(Angle.x);
            targetVec.z = cosf(Angle.y + Angle.x);


        }

        if (input->IsKeyDown(DIK_D) || input->IsKeyDown(DIK_A) || input->IsKeyDown(DIK_W) || input->IsKeyDown(DIK_S))
        {
            XMFLOAT3 move = { 0.0f,0.0f,0.0f };
            if (input->IsKeyDown(DIK_W)) { move.z += 1.0f; }
            else if (input->IsKeyDown(DIK_S)) { move.z -= 1.0f; }

            //if (input.IsKeyDown(DIK_D)) { move.x += 1.0f; }
            //else if (input.IsKeyDown(DIK_A)) { move.x-= 1.0f; }

            matView.eye.x += targetVec.x * move.z;
            matView.eye.y += targetVec.y * move.z;
            matView.eye.z += targetVec.z * move.z;



        }

        matView.target.x = matView.eye.x + targetVec.x;
        matView.target.y = matView.eye.y + targetVec.y;
        matView.target.z = matView.eye.z + targetVec.z;


        matView.UpDateMatrixView();

        //for (int i = 0; i < _countof(object3D); i++)
        //{
        //    object3D[i].Updata(matView, matProjection,true);
        //}

        //for (int i = 0; i < _countof(Rales); i++)
        //{
        //    Rales[i].Updata(matView, matProjection);
        //}

        triangle.Updata(matView, matProjection, true);

#pragma endregion 更新処理

#pragma region 描画処理
        //バックバッファの番号を取得（2つなので0番か1番）--------------------------
        UINT bbIndex = dx->swapchain->GetCurrentBackBufferIndex();
        //-----------------------------------

        // １．リソースバリアで書き込み可能に変更----
#pragma region １．リソースバリアで書き込み可能に変更

        D3D12_RESOURCE_BARRIER barrierDesc{};
        barrierDesc.Transition.pResource = dx->backBuffers[bbIndex].Get(); // バックバッファを指定
        barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // 表示から
        barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画
        dx->commandList->ResourceBarrier(1, &barrierDesc);

#pragma endregion 1．リソースバリアで書き込み可能に変更
        //--------------------------

        // ２．描画先指定----------------
#pragma region ２．描画先指定

// レンダーターゲットビュー用ディスクリプタヒープのハンドルを取得
        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = dx->rtvHeaps->GetCPUDescriptorHandleForHeapStart();
        rtvHandle.ptr += bbIndex * dx->device->GetDescriptorHandleIncrementSize(dx->rtvHeapDesc.Type);
        D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = depth.dsvHeap->GetCPUDescriptorHandleForHeapStart();
        dx->commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

#pragma endregion 2．描画先指定
        //-------------------
        
        //３．画面クリア-------------
#pragma region 3.画面クリア
        dx->commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
        dx->commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
#pragma endregion 3.画面クリア
        //---------------------------

        //描画コマンド------------------
#pragma region 描画コマンド
            //ビューポートの設定コマンド-----------------------------
#pragma region ビューポートの設定コマンド

        D3D12_VIEWPORT viewport{};

        viewport.Width = dxWindow->window_width;
        viewport.Height = dxWindow->window_height;
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

        dx->commandList->RSSetViewports(1, &viewport);

#pragma endregion ビューポートの設定コマンド
        //------------------------------

        //シザー矩形の設定コマンド-----------------
#pragma region シザー矩形の設定コマンド

        D3D12_RECT scissorrect{};

        scissorrect.left = 0;                                       // 切り抜き座標左
        scissorrect.right = scissorrect.left + dxWindow->window_width;        // 切り抜き座標右
        scissorrect.top = 0;                                        // 切り抜き座標上
        scissorrect.bottom = scissorrect.top + dxWindow->window_height;       // 切り抜き座標下

        dx->commandList->RSSetScissorRects(1, &scissorrect);

#pragma endregion シザー矩形の設定コマンド
        //------------------

        dx->commandList->SetPipelineState(pipleline.pipelinestate.Get());
        dx->commandList->SetGraphicsRootSignature(rootsignature.rootsignature.Get());
        

        //プリミティブ形状の設定コマンド（三角形リスト）--------------------------
        dx->commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        
        //定数バッファビュー(CBV)の設定コマンド
        dx->commandList->SetGraphicsRootConstantBufferView(0, triangle.model.material.constBuffMaterialB1->GetGPUVirtualAddress());

        //SRVヒープの設定コマンド
        dx->commandList->SetDescriptorHeaps(1, descriptor.srvHeap.GetAddressOf());

        //SRVヒープの先頭アドレスを取得
        D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor.srvHeap->GetGPUDescriptorHandleForHeapStart();

        //SRVヒープの先頭にあるSRVをパラメータ1番に設定
        dx->commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

        //for (int i = 0; i < _countof(object3D); i++)
        //{
        //    object3D[i].Draw(*dx);
        //}

        //for (int i = 0; i < _countof(Rales); i++)
        //{
        //    Rales[i].Draw(*dx);
        //}

        triangle.Draw(*dx);

#pragma endregion 描画コマンド
        //----------------------

        // ５．リソースバリアを戻す--------------
#pragma region 5.リソースバリアを戻す

        barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画
        barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;   // 表示に
        dx->commandList->ResourceBarrier(1, &barrierDesc);

#pragma endregion 5.リソースバリアを戻す
        //--------------------

        // 命令のクローズ-----------------------------------
        dx->result = dx->commandList->Close();
        assert(SUCCEEDED(dx->result) && "命令クローズ段階でのエラー");
        //------------
        

        // コマンドリストの実行-------------------------------------
#pragma region コマンドリスト実行
        ID3D12CommandList* commandLists[] = { dx->commandList.Get() }; // コマンドリストの配列
        dx->commandQueue->ExecuteCommandLists(1, commandLists);

        // バッファをフリップ（裏表の入替え)-----------------------
       dx->result =  dx->swapchain->Present(1, 0);
       assert(SUCCEEDED(dx->result) && "バッファフリップ段階でのエラー");
        //-----------------

#pragma region コマンド実行完了待ち
    // コマンドリストの実行完了を待つ
        dx->commandQueue->Signal(dx->fence.Get(), ++dx->fenceVal);
        if (dx->fence->GetCompletedValue() != dx->fenceVal)
        {
            HANDLE event = CreateEvent(nullptr, false, false, nullptr);
            dx->fence->SetEventOnCompletion(dx->fenceVal, event);
            WaitForSingleObject(event, INFINITE);
            CloseHandle(event);
        }
#pragma endregion コマンド実行完了待ち

        //キューをクリア
        dx->result = dx->commandAllocator->Reset(); // キューをクリア
        assert(SUCCEEDED(dx->result) && "キュークリア段階でのエラー");

        //再びコマンドリストをためる準備
        dx->result = dx->commandList->Reset(dx->commandAllocator.Get(), nullptr);  // 再びコマンドリストを貯める準備
        assert(SUCCEEDED(dx->result) && "コマンドリスト再貯蓄準備段階でのエラー");

#pragma endregion コマンドリスト実行
//------------------

#pragma endregion 描画処理


    }
#pragma endregion ゲームループ
    //---------------------------------
    delete dxWindow;
    delete dx;
    delete input;
    //delete textureFile;
    delete mipmap;
    delete imageData;

    //_CrtDumpMemoryLeaks();
	return 0;
}
