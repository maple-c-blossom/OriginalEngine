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

#pragma endregion using namespace

//頂点データ構造体-------------------------------------
typedef struct Vertex
{
    XMFLOAT3 pos;//xyz座標
    XMFLOAT3 normal;//法線ベクトル
    XMFLOAT2 uv;//uv座標
};
//--------------------------------------

// Windowsアプリでのエントリーポイント(main関数) 
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) 
{  

    DxWindow dxWindow;

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
    Dx12 dx(dxWindow);
    //inputクラス生成
    Input input(dx.result,dxWindow.window,dxWindow.hwnd);

#pragma endregion 


#pragma region 描画初期化処理

    //深度バッファ----
    Depth depth(dxWindow, dx);
    //-------

    //3Dオブジェクトマテリアルの生成-------------------
    ObjectMaterial objMaterial;
    objMaterial.Init(dx);
    //---------------------

    //3Dオブジェクトの生成-------------------
#pragma region 3Dオブジェクトの生成

     const size_t objectNum = 50;
     Object3d object3D[objectNum];
     for (int i = 0; i < objectNum; i++)
     {
         object3D[i].Init(dx);
         if (i > 0)
         {
             object3D[i].parent = &object3D[i - 1];

             object3D[i].scale = { 0.9f,0.9f,0.9f };

             object3D[i].rotasion = { 0.0f,0.0f,XMConvertToRadians(30.0f) };

             object3D[i].position = { 0.0f,0.0f,-0.8f };
         }
     }

#pragma endregion 3Dオブジェクトの生成
    //----------------------

     //行列-----------------------
#pragma region 行列
        //ビュー変換行列
        View matView;
        matView.CreateMatrixView(XMFLOAT3(0.0f, 0.0f, -100.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
        //射影変換行列
        Projection matProjection;
         matProjection.CreateMatrixProjection(XMConvertToRadians(45.0f),(float)dxWindow.window_width / dxWindow.window_height, 0.1f, 1000.0f);
#pragma endregion 行列
     //---------------------

     //画像ファイル--------------------
#pragma region 画像ファイル
     TexMetadata metadata{};
     ScratchImage scratchImg{};

     dx.result = LoadFromWICFile(L"Resources\\tori.png", WIC_FLAGS_NONE, &metadata, scratchImg);
#pragma endregion 画像ファイル
     //----------------------------

     //ミップマップの生成-------------
#pragma region ミップマップの生成

     ScratchImage mipChain{};
     //ミップマップ生成
     dx.result = GenerateMipMaps(scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(), TEX_FILTER_DEFAULT, 0, mipChain);
     if (SUCCEEDED(dx.result))
     {
         scratchImg = std::move(mipChain);
         metadata = scratchImg.GetMetadata();
     }
     metadata.format = MakeSRGB(metadata.format);
#pragma endregion ミップマップの生成
     //---------------

     //画像イメージデータの作成----------------------
#pragma region 画像イメージデータの作成
     //横方向ピクセル数
     const size_t textureWidth = 256;
     //縦方向ピクセル数
     const size_t textureHeight = 256;
     //配列の要素数
     const size_t imageDataCount = textureWidth * textureHeight;
     //画像イメージデータの配列
     XMFLOAT4* imageData = new XMFLOAT4[imageDataCount];

     for (int i = 0; i < imageDataCount; i++)
     {
         imageData[i].x = 1.0f;//R
         imageData[i].y = 0.0f;//G
         imageData[i].z = 0.0f;//B
         imageData[i].w = 1.0f;//A
     }

#pragma endregion 画像イメージデータの作成
     //------------------------------------

#pragma region テクスチャバッファ設定

     D3D12_HEAP_PROPERTIES texHeapProp{};

     texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
     texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
     texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

     D3D12_RESOURCE_DESC texresDesc{};
     texresDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
     texresDesc.Format = metadata.format;
     texresDesc.Width = metadata.width;
     texresDesc.Height = (UINT)metadata.height;
     texresDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
     texresDesc.MipLevels = (UINT16)metadata.mipLevels;
     texresDesc.SampleDesc.Count = 1;


#pragma endregion テクスチャバッファ設定


#pragma region テクスチャバッファの生成
     ComPtr<ID3D12Resource> texbuff = nullptr;
     dx.result = dx.device->CreateCommittedResource(&texHeapProp, D3D12_HEAP_FLAG_NONE, &texresDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&texbuff));
#pragma endregion テクスチャバッファの生成

     for (size_t i = 0; i < metadata.mipLevels; i++)
     {
         //ミップマップレベルを指定してイメージを取得
         const Image* img = scratchImg.GetImage(i, 0, 0);
         //テクスチャバッファにデータ転送
         dx.result = texbuff->WriteToSubresource((UINT) i, nullptr,img->pixels, (UINT)img->rowPitch,(UINT)img->slicePitch);
         assert(SUCCEEDED(dx.result));
     }

         //デスクリプタヒープの生成-------------------------
#pragma region デスクリプタヒープの生成

     const size_t kMaxSRVCount = 2056;

    //定数バッファ用のデスクリプタヒープ

     //設定構造体
     D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};
     srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
     srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; //シェーダーから見えるように
     srvHeapDesc.NumDescriptors = kMaxSRVCount;//定数バッファの数

     //デスクリプタヒープの生成  
     ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;
     dx.result = dx.device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));

     D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();
#pragma endregion デスクリプタヒープの生成
    //-------------------------------

     //シェーダーリソースビューの作成------------------------------
#pragma region シェーダーリソースビューの作成
     //シェーダーリソースビュー設定
     D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

     srvDesc.Format = texresDesc.Format;
     srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
     srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
     srvDesc.Texture2D.MipLevels = texresDesc.MipLevels;

     //ヒープの二番目にシェーダーリソースビュー作成
     dx.device->CreateShaderResourceView(texbuff.Get(), &srvDesc, srvHandle);

#pragma endregion シェーダーリソースビューの作成
     //----------------------------

     //デスクリプタレンジの設定--------------------------------
#pragma region デスクリプタレンジの設定
     D3D12_DESCRIPTOR_RANGE descriptorRange{};
     descriptorRange.NumDescriptors = 1;
     descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
     descriptorRange.BaseShaderRegister = 0;
     descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
#pragma endregion デスクリプタレンジの設定
     //-----------------------------------------

     //ルートパラメータの設定---------------------------
#pragma region ルートパラメータの設定

     D3D12_ROOT_PARAMETER rootparams[3] = {};
     //定数バッファ0番
     rootparams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//種類
     rootparams[0].Descriptor.ShaderRegister = 0;//定数バッファ番号
     rootparams[0].Descriptor.RegisterSpace = 0;//デフォルト値
     rootparams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てのシェーダーから見える
     //テクスチャレジスタ0番
     rootparams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
     rootparams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;
     rootparams[1].DescriptorTable.NumDescriptorRanges = 1;
     rootparams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
     //定数バッファ1番
     rootparams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
     rootparams[2].Descriptor.ShaderRegister = 1;
     rootparams[2].Descriptor.RegisterSpace = 0;
     rootparams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
#pragma endregion ルートパラメータの設定
     //------------------------

     //頂点データ---------------------------------
#pragma region 頂点データ
    Vertex vertices[] =
    {
        //前
        {{-5.0f,-5.0f,-5.0f} ,{} ,{0.0f,1.0f}},// 左下(x,y,z,u,v)
        {{-5.0f,5.0f,-5.0f}  ,{} ,{0.0f,0.0f}},// 左上
        {{5.0f,-5.0f,-5.0f}  ,{} ,{1.0f,1.0f}},// 右下
        {{5.0f,5.0f,-5.0f}   ,{} ,{1.0f,0.0f}},// 右上
        //後ろ
        {{-5.0f,5.0f,5.0f}  ,{} ,{0.0f,0.0f}},// 左上
        {{-5.0f,-5.0f,5.0f} ,{} ,{0.0f,1.0f}},// 左下(x,y,z,u,v)
        {{5.0f,5.0f,5.0f}   ,{} ,{1.0f,0.0f}},// 右上
        {{5.0f,-5.0f,5.0f}  ,{} ,{1.0f,1.0f}},// 右下
        //左
        {{-5.0f,-5.0f,-5.0f} ,{} ,{0.0f,1.0f}},// 左下(x,y,z,u,v)
        {{-5.0f,-5.0f,5.0f}  ,{} ,{0.0f,0.0f}},// 左上
        {{-5.0f,5.0f,-5.0f}  ,{} ,{1.0f,1.0f}},// 右下
        {{-5.0f,5.0f,5.0f}   ,{} ,{1.0f,0.0f}},// 右上
        //右
        {{5.0f,-5.0f,5.0f}  ,{} ,{0.0f,0.0f}},// 左上
        {{5.0f,-5.0f,-5.0f} ,{} ,{0.0f,1.0f}},// 左下(x,y,z,u,v)
        {{5.0f,5.0f,5.0f}   ,{} ,{1.0f,0.0f}},// 右上
        {{5.0f,5.0f,-5.0f}  ,{} ,{1.0f,1.0f}},// 右下
        //上
        {{-5.0f,-5.0f,-5.0f} ,{} ,{0.0f,1.0f}},// 左下(x,y,z,u,v)
        {{5.0f,-5.0f,-5.0f}  ,{} ,{0.0f,0.0f}},// 左上
        {{-5.0f,-5.0f,5.0f}  ,{} ,{1.0f,1.0f}},// 右下
        {{5.0f,-5.0f,5.0f}   ,{} ,{1.0f,0.0f}},// 右上
        //下
        {{5.0f,5.0f,-5.0f}  ,{} ,{0.0f,0.0f}},// 左上
        {{-5.0f,5.0f,-5.0f} ,{} ,{0.0f,1.0f}},// 左下(x,y,z,u,v)
        {{5.0f,5.0f,5.0f}   ,{} ,{1.0f,0.0f}},// 右上
        {{-5.0f,5.0f,5.0f}  ,{} ,{1.0f,1.0f}},// 右下
    };





     UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));
#pragma endregion 頂点データ
     //--------------------------
     
     //頂点インデックス---------------------------
#pragma region 頂点インデックス
      unsigned short indices[]
      {
          //前
          0,1,2,
          2,1,3,
          //後ろ
          4,5,6,
          6,5,7,
          //左
          8,9,10,
          10,9,11,
          //右
          12,13,14,
          14,13,15,
          //上
          16,17,18,
          18,17,19,
          //下
          20,21,22,
          22,21,23,
      };
#pragma endregion 頂点インデックス
     //--------------------------

     //インデックスバッファの設定-------------------------
#pragma region インデックスの設定
     //インデックスデータ全体のサイズ
     UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

     objMaterial.Resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
     objMaterial.Resdesc.Width = sizeIB;
     objMaterial.Resdesc.Height = 1;
     objMaterial.Resdesc.DepthOrArraySize = 1;
     objMaterial.Resdesc.MipLevels = 1;
     objMaterial.Resdesc.SampleDesc.Count = 1;
     objMaterial.Resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

#pragma endregion インデックスの設定
     //------------------------

#pragma region インデックスバッファ生成

     ComPtr<ID3D12Resource> indexBuff = nullptr;
         //インデックスバッファの生成-----------------------------
     dx.result = dx.device->CreateCommittedResource(
         &objMaterial.HeapProp,
         D3D12_HEAP_FLAG_NONE,
         &objMaterial.Resdesc,
         D3D12_RESOURCE_STATE_GENERIC_READ,
         nullptr,
         IID_PPV_ARGS(&indexBuff)
     );

#pragma endregion インデックスバッファ生成

     //インデックスバッファへのデータ転送------------------------------
#pragma region インデックスバッファへのデータ転送

     //GPU上のバッファに対応した仮想メモリを取得----------------------------
     uint16_t* indexMap = nullptr;
     dx.result = indexBuff->Map(0, nullptr, (void**)&indexMap);
     //---------------------------------------

     //全インデックスに対して-------------------------
     for (int i = 0; i < _countof(indices); i++)
     {
         indexMap[i] = indices[i];
     }
     //-----------------------

     //繋がりを解除---------------------
     indexBuff->Unmap(0, nullptr);
     //------------------------

#pragma endregion インデックスバッファへのデータ転送
    //-------------------------------------

     //インデックスバッファビューの作成-----------------------------------
#pragma region インデックスバッファビューの作成
     D3D12_INDEX_BUFFER_VIEW ibView{};
     ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
     ibView.Format = DXGI_FORMAT_R16_UINT;
     ibView.SizeInBytes = sizeIB;
#pragma endregion インデックスバッファビューの作成
     //------------------------------------------

     //頂点バッファ---------------
#pragma region 頂点バッファの設定
    D3D12_HEAP_PROPERTIES heapprop{};   // ヒープ設定
    objMaterial.HeapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用

    D3D12_RESOURCE_DESC resdesc{};  // リソース設定
    objMaterial.Resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    objMaterial.Resdesc.Width = sizeVB; // 頂点データ全体のサイズ
    objMaterial.Resdesc.Height = 1;
    objMaterial.Resdesc.DepthOrArraySize = 1;
    objMaterial.Resdesc.MipLevels = 1;
    objMaterial.Resdesc.SampleDesc.Count = 1;
    objMaterial.Resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
#pragma endregion 頂点バッファの設定
     //----------------------------------

     // 頂点バッファの生成----------------------------
#pragma region 頂点バッファの生成
     ComPtr<ID3D12Resource> vertBuff = nullptr;
     dx.result = dx.device->CreateCommittedResource(
         &objMaterial.HeapProp, // ヒープ設定
         D3D12_HEAP_FLAG_NONE,
         &objMaterial.Resdesc, // リソース設定
         D3D12_RESOURCE_STATE_GENERIC_READ,
         nullptr,
         IID_PPV_ARGS(&vertBuff));

     assert(SUCCEEDED(dx.result));
#pragma endregion 頂点バッファの生成
     //-------------------------

     //法線ベクトル計算---------------------------
#pragma region 法線ベクトル計算
     for (int i = 0; i < _countof(indices) / 3; i++)
     {
         //三角形1つごとに計算

         //三角形のインデックスを取り出して、一時的な変数に入れる
         unsigned short index0 = indices[i * 3 + 0];
         unsigned short index1 = indices[i * 3 + 1];
         unsigned short index2 = indices[i * 3 + 2];

         //三角形を構成する頂点座標
         XMVECTOR p0 = XMLoadFloat3(&vertices[index0].pos);
         XMVECTOR p1 = XMLoadFloat3(&vertices[index1].pos);
         XMVECTOR p2 = XMLoadFloat3(&vertices[index2].pos);

         //p0->p1ベクトル、p0->p2ベクトルを計算（ベクトルの減算）
         XMVECTOR v1 = XMVectorSubtract(p1, p0);
         XMVECTOR v2 = XMVectorSubtract(p2, p0);

         //外積は両方から垂直なベクトル
         XMVECTOR normal = XMVector3Cross(v1, v2);

         //正規化（長さを一にする)
         normal = XMVector3Normalize(normal);

         //求めた法線を頂点データに代入
         XMStoreFloat3(&vertices[index0].normal, normal);
         XMStoreFloat3(&vertices[index1].normal, normal);
         XMStoreFloat3(&vertices[index2].normal, normal);

     }
#pragma endregion 法線ベクトルを計算
     //-------------------------

     // 頂点バッファへのデータ転送------------
#pragma region 頂点バッファへのデータ転送
     Vertex* vertMap = nullptr;
     dx.result = vertBuff->Map(0, nullptr, (void**)&vertMap);
     assert(SUCCEEDED(dx.result));

     // 全頂点に対して
     for (int i = 0; i < _countof(vertices); i++)
     {
         vertMap[i] = vertices[i];   // 座標をコピー
     }

     // マップを解除
     vertBuff->Unmap(0, nullptr);
#pragma endregion 頂点バッファへのデータ転送
     //--------------------------------------

     // 頂点バッファビューの作成--------------------------
#pragma region 頂点バッファビューの作成
     D3D12_VERTEX_BUFFER_VIEW vbView{};

     vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
     vbView.SizeInBytes = sizeVB;
     vbView.StrideInBytes = sizeof(vertices[0]);
#pragma endregion 頂点バッファビューの作成
     //-----------------------------------

       //シェーダーオブジェクト宣言-------------------------------------------
#pragma region シェーダーオブジェクト宣言
     ComPtr<ID3DBlob> vsBlob = nullptr; // 頂点シェーダオブジェクト
     ComPtr<ID3DBlob> psBlob = nullptr; // ピクセルシェーダオブジェクト
     ComPtr<ID3DBlob> errorBlob = nullptr; // エラーオブジェクト
#pragma endregion シェーダーオブジェクト宣言
//---------------------------------

// 頂点シェーダの読み込みとコンパイル--------------------------------
#pragma region 頂点シェーダの読み込みとコンパイル

     dx.result = D3DCompileFromFile(
         L"BasicVS.hlsl",  // シェーダファイル名
         nullptr,
         D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
         "main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
         D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
         0,
         &vsBlob, &errorBlob);

#pragma endregion 頂点シェーダの読み込みとコンパイル
     //------------------------------------------


     //  シェーダーのエラーに関する出力部分-----------------
#pragma region シェーダーのエラーに関する出力部分

     if (FAILED(dx.result)) {
         // errorBlobからエラー内容をstring型にコピー
         string error;
         error.resize(errorBlob->GetBufferSize());

         copy_n((char*)errorBlob->GetBufferPointer(),
             errorBlob->GetBufferSize(),
             error.begin());
         error += "\n";
         // エラー内容を出力ウィンドウに表示
         OutputDebugStringA(error.c_str());
         assert(0);
     }

#pragma endregion シェーダーのエラーに関する出力部分
     //-----------------------------------

     // ピクセルシェーダの読み込みとコンパイル-------------------------------
#pragma region ピクセルシェーダの読み込みとコンパイル

     dx.result = D3DCompileFromFile(
         L"BasicPS.hlsl",   // シェーダファイル名
         nullptr,
         D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
         "main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
         D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
         0,
         &psBlob, &errorBlob);

#pragma endregion ピクセルシェーダの読み込みとコンパイル
     //--------------------------------

     //  シェーダーのエラーに関する出力部分-----------------
#pragma region シェーダーのエラーに関する出力部分

     if (FAILED(dx.result)) {
         // errorBlobからエラー内容をstring型にコピー
         string error;
         error.resize(errorBlob->GetBufferSize());

         copy_n((char*)errorBlob->GetBufferPointer(),
             errorBlob->GetBufferSize(),
             error.begin());
         error += "\n";
         // エラー内容を出力ウィンドウに表示
         OutputDebugStringA(error.c_str());
         assert(0);
     }

#pragma endregion シェーダーのエラーに関する出力部分
     //-----------------------------------

      // 頂点レイアウト------------------
#pragma region 頂点レイアウト

     D3D12_INPUT_ELEMENT_DESC inputLayout[] =
     {
         {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}, // (1行で書いたほうが見やすい)
         {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},//法線ベクトル
         {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}//uv座標
     };

#pragma endregion 頂点レイアウト
     //--------------------

     // グラフィックスパイプライン設定-----------
     D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipelineDesc{};
     //-------------------------

     //頂点シェーダ、ピクセルシェーダをパイプラインに設定-----------------------------
#pragma region 頂点シェーダとピクセルシェーダをパイプラインに設定

     gpipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
     gpipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
     gpipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
     gpipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

#pragma endregion 頂点シェーダとピクセルシェーダをパイプラインに設定
     //-----------------------------------

     //サンプルマスクとラスタライザステートの設定------------------------------------
#pragma region サンプルマスクとラスタライザステートの設定

     gpipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
     gpipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;  // 背面カリング
     gpipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
     gpipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

#pragma endregion サンプルマスクとラスタライザステートの設定
//------------------------------------


      //ブレンドステートの設定-------------------------------
#pragma region ブレンドステートの設定
//gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;  // RBGA全てのチャンネルを描画
     D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipelineDesc.BlendState.RenderTarget[0];
     blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//標準設定

     //共通設定
     blenddesc.BlendEnable = true;
     blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
     blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
     blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

     //半透明合成
     blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
     blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
     blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

     //加算合成
     //#pragma region 加算合成
     //blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
     //blenddesc.SrcBlend = D3D12_BLEND_ONE;
     //blenddesc.DestBlend = D3D12_BLEND_ONE;
     //#pragma endregion

     //減算合成
   /*  #pragma region 減算合成
     blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
     blenddesc.SrcBlend = D3D12_BLEND_ONE;
     blenddesc.DestBlend = D3D12_BLEND_ONE;
     #pragma endregion*/

     ////色反転
     //blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
     //blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
     //blenddesc.DestBlend = D3D12_BLEND_ZERO;


#pragma endregion ブレンドステートの設定
     //--------------------------

     //頂点レイアウトの設定------------------
#pragma region 頂点レイアウトの設定

     gpipelineDesc.InputLayout.pInputElementDescs = inputLayout;
     gpipelineDesc.InputLayout.NumElements = _countof(inputLayout);

#pragma endregion 頂点レイアウトの設定
     //----------------------------

     //図形の形状を三角形に設定-------------------------
     gpipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
     //------------------

     //その他の設定----------------
#pragma region その他の設定

     gpipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
     gpipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0〜255指定のRGBA
     gpipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

#pragma endregion その他の設定
    //----------------

     depth.SetDepthStencilState(gpipelineDesc);

   //テクスチャサンプラーの設定-----------------------
#pragma region テクスチャサンプラーの設定

     D3D12_STATIC_SAMPLER_DESC samplerDesc{};

     samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
     samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
     samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
     samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
     samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
     samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
     samplerDesc.MinLOD = 0.0f;
     samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
     samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

#pragma endregion テクスチャサンプラーの設定
   //----------------------------------

     //ルートシグネチャの生成--------------------------
#pragma region ルートシグネチャの生成

     ComPtr <ID3D12RootSignature> rootsignature;

     D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
     rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
     rootSignatureDesc.pParameters = rootparams; //ルートパラメータの先頭アドレス
     rootSignatureDesc.NumParameters = _countof(rootparams); //ルートパラメータ数
     rootSignatureDesc.pStaticSamplers = &samplerDesc;
     rootSignatureDesc.NumStaticSamplers = 1;


     ComPtr<ID3DBlob> rootSigBlob = nullptr;
     dx.result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
     dx.result = dx.device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));

     assert(SUCCEEDED(dx.result));

     // パイプラインにルートシグネチャをセット
     gpipelineDesc.pRootSignature = rootsignature.Get();

#pragma endregion ルートシグネチャの生成
     //--------------------------------

    //パイプラインステートの生成-------------------------
#pragma region パイプラインステートの生成

     ComPtr<ID3D12PipelineState> pipelinestate = nullptr;
     dx.result = dx.device->CreateGraphicsPipelineState(&gpipelineDesc, IID_PPV_ARGS(&pipelinestate));
     assert(SUCCEEDED(dx.result));
#pragma endregion パイプラインステートの生成
     //-----------------------------

     float clearColor[] = { 0.1f,0.25f, 0.5f,0.0f }; // 青っぽい色

#pragma endregion

     //ゲームループ用変数--------------------------------
#pragma region ゲームループ用変数
     float angle = 0.0f;

#pragma endregion ゲームループ用変数
     //--------------------------
     
     //ゲームループ-------------------------------------
#pragma region ゲームループ
    while (true)
    {
        input.UpDateInit(dx.result);

        dxWindow.messageUpdate();

        if (input.IsKeyDown(DIK_ESCAPE) || dxWindow.breakFlag)
        {
            break;
        }

#pragma region 更新処理
        if (input.IsKeyDown(DIK_D) || input.IsKeyDown(DIK_A))
        {
            if (input.IsKeyDown(DIK_D)) { angle += XMConvertToRadians(1.0f); }
            else if (input.IsKeyDown(DIK_A)) { angle -= XMConvertToRadians(1.0f); }

            matView.eye.x = -100.0f * sinf(angle);
            matView.eye.z = -100.0f * cosf(angle);
            matView.UpDateMatrixView();

        }

        if (input.IsKeyDown(DIK_UP) || input.IsKeyDown(DIK_DOWN) || input.IsKeyDown(DIK_LEFT) || input.IsKeyDown(DIK_RIGHT))
        {
            if (input.IsKeyDown(DIK_UP)) { object3D[0].position.z += 1.0f; }
            else if (input.IsKeyDown(DIK_DOWN)) { object3D[0].position.z -= 1.0f; }

            if (input.IsKeyDown(DIK_RIGHT)) { object3D[0].position.x += 1.0f; }
            else if (input.IsKeyDown(DIK_LEFT)) { object3D[0].position.x -= 1.0f; }
        }


        for (int i = 0; i < _countof(object3D); i++)
        {
            object3D[i].Updata(matView, matProjection);
        }

#pragma endregion 更新処理

#pragma region 描画処理
        //バックバッファの番号を取得（2つなので0番か1番）--------------------------
        UINT bbIndex = dx.swapchain->GetCurrentBackBufferIndex();
        //-----------------------------------

        // １．リソースバリアで書き込み可能に変更----
#pragma region １．リソースバリアで書き込み可能に変更

        D3D12_RESOURCE_BARRIER barrierDesc{};
        barrierDesc.Transition.pResource = dx.backBuffers[bbIndex].Get(); // バックバッファを指定
        barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // 表示から
        barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画
        dx.commandList->ResourceBarrier(1, &barrierDesc);

#pragma endregion 1．リソースバリアで書き込み可能に変更
        //--------------------------

        // ２．描画先指定----------------
#pragma region ２．描画先指定

// レンダーターゲットビュー用ディスクリプタヒープのハンドルを取得
        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = dx.rtvHeaps->GetCPUDescriptorHandleForHeapStart();
        rtvHandle.ptr += bbIndex * dx.device->GetDescriptorHandleIncrementSize(dx.rtvHeapDesc.Type);
        D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = depth.dsvHeap->GetCPUDescriptorHandleForHeapStart();
        dx.commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

#pragma endregion 2．描画先指定
        //-------------------
        
        //３．画面クリア-------------
#pragma region 3.画面クリア
        dx.commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
        dx.commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
#pragma endregion 3.画面クリア
        //---------------------------

        //描画コマンド------------------
#pragma region 描画コマンド
            //ビューポートの設定コマンド-----------------------------
#pragma region ビューポートの設定コマンド

        D3D12_VIEWPORT viewport{};

        viewport.Width = dxWindow.window_width;
        viewport.Height = dxWindow.window_height;
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

        dx.commandList->RSSetViewports(1, &viewport);

#pragma endregion ビューポートの設定コマンド
        //------------------------------

        //シザー矩形の設定コマンド-----------------
#pragma region シザー矩形の設定コマンド

        D3D12_RECT scissorrect{};

        scissorrect.left = 0;                                       // 切り抜き座標左
        scissorrect.right = scissorrect.left + dxWindow.window_width;        // 切り抜き座標右
        scissorrect.top = 0;                                        // 切り抜き座標上
        scissorrect.bottom = scissorrect.top + dxWindow.window_height;       // 切り抜き座標下

        dx.commandList->RSSetScissorRects(1, &scissorrect);

#pragma endregion シザー矩形の設定コマンド
        //------------------

        dx.commandList->SetPipelineState(pipelinestate.Get());
        dx.commandList->SetGraphicsRootSignature(rootsignature.Get());
        

        //プリミティブ形状の設定コマンド（三角形リスト）--------------------------
        dx.commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        
        //定数バッファビュー(CBV)の設定コマンド
        dx.commandList->SetGraphicsRootConstantBufferView(0, objMaterial.constBuffMaterial->GetGPUVirtualAddress());

        //SRVヒープの設定コマンド
        dx.commandList->SetDescriptorHeaps(1, srvHeap.GetAddressOf());

        //SRVヒープの先頭アドレスを取得
        D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();

        //SRVヒープの先頭にあるSRVをパラメータ1番に設定
        dx.commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

        for (int i = 0; i < _countof(object3D); i++)
        {
            object3D[i].Draw(dx, vbView, ibView, _countof(indices));
        }

#pragma endregion 描画コマンド
        //----------------------

        // ５．リソースバリアを戻す--------------
#pragma region 5.リソースバリアを戻す

        barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画
        barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;   // 表示に
        dx.commandList->ResourceBarrier(1, &barrierDesc);

#pragma endregion 5.リソースバリアを戻す
        //--------------------

        // 命令のクローズ-----------------------------------
        dx.result = dx.commandList->Close();
        assert(SUCCEEDED(dx.result));
        //------------
        

        // コマンドリストの実行-------------------------------------
#pragma region コマンドリスト実行
        ID3D12CommandList* commandLists[] = { dx.commandList.Get() }; // コマンドリストの配列
        dx.commandQueue->ExecuteCommandLists(1, commandLists);

        // バッファをフリップ（裏表の入替え)-----------------------
       dx.result =  dx.swapchain->Present(1, 0);
       assert(SUCCEEDED(dx.result));
        //-----------------

#pragma region コマンド実行完了待ち
    // コマンドリストの実行完了を待つ
        dx.commandQueue->Signal(dx.fence.Get(), ++dx.fenceVal);
        if (dx.fence->GetCompletedValue() != dx.fenceVal)
        {
            HANDLE event = CreateEvent(nullptr, false, false, nullptr);
            dx.fence->SetEventOnCompletion(dx.fenceVal, event);
            WaitForSingleObject(event, INFINITE);
            CloseHandle(event);
        }
#pragma endregion コマンド実行完了待ち

        //キューをクリア
        dx.result = dx.commandAllocator->Reset(); // キューをクリア
        assert(SUCCEEDED(dx.result));

        //再びコマンドリストをためる準備
        dx.result = dx.commandList->Reset(dx.commandAllocator.Get(), nullptr);  // 再びコマンドリストを貯める準備
        assert(SUCCEEDED(dx.result));

#pragma endregion コマンドリスト実行
//------------------

#pragma endregion 描画処理


    }
#pragma endregion ゲームループ
    //---------------------------------
	return 0;
}
