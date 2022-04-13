#pragma region include

#include <d3dcompiler.h>
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <string>
#include <DirectXMath.h>
#include <dinput.h>
#include <wrl.h>

#pragma endregion include

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

// ウィンドウプロシージャ 
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {  // メッセージに応じてゲーム固有の処理を行う 
	switch (msg) {
		// ウィンドウが破棄された 
	case WM_DESTROY:
		// OSに対して、アプリの終了を伝える 
		PostQuitMessage(0);
		return 0;
	}
	// 標準のメッセージ処理を行う 
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

//キーボード初期化-------------------------------
void KeyInit(BYTE* key, BYTE* oldkey, int ArraySize)
{
    for (int i = 0; i < ArraySize; i++)
    {
        oldkey[i] = key[i];
        key[i] = 0;
    }
}
//---------------------

//入力関数-----------------------------------
#pragma region 入力関数
//押している状態
bool IsKeyDown(int keyNum, BYTE* key)
{
    if (key[keyNum])
    {
        return true;
    }
    return false;
};
//押していない状態
bool IsKeyNDown(int keyNum, BYTE* key)
{
    if (!key[keyNum])
    {
        return true;
    }
    return false;
};
//長押し状態
bool IsKeyPress(int keyNum, BYTE* key, BYTE* oldkey)
{
    if (key[keyNum] && oldkey[keyNum])
    {
        return true;
    }
    return false;
};
//押した瞬間
bool IsKeyTrigger(int keyNum, BYTE* key, BYTE* oldkey)
{
    if (key[keyNum] && !oldkey[keyNum])
    {
        return true;
    }
    return false;
};
//離した瞬間
bool IsKeyRelease(int keyNum, BYTE* key, BYTE* oldkey)
{
    if (!key[keyNum] && oldkey[keyNum])
    {
        return true;
    }
    return false;
};
#pragma endregion 入力関数
//--------------------


// Windowsアプリでのエントリーポイント(main関数) 
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) 
{  
	

  // ウィンドウの生成（前処理-----------------------
#pragma region ウィンドウ生成
    // ウィンドウサイズ
    const int window_width = 1280;  // 横幅
    const int window_height = 720;  // 縦幅

    WNDCLASSEX w{}; // ウィンドウクラスの設定
    w.cbSize = sizeof(WNDCLASSEX);
    w.lpfnWndProc = (WNDPROC)WindowProc; // ウィンドウプロシージャを設定
    w.lpszClassName = L"DirectXGame"; // ウィンドウクラス名
    w.hInstance = GetModuleHandle(nullptr); // ウィンドウハンドル
    w.hCursor = LoadCursor(NULL, IDC_ARROW); // カーソル指定

    // ウィンドウクラスをOSに登録
    RegisterClassEx(&w);
    // ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
    RECT wrc = { 0, 0, window_width, window_height };
    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); // 自動でサイズ補正


    // ウィンドウオブジェクトの生成
    HWND hwnd = CreateWindow(w.lpszClassName, // クラス名
        L"DirectX12Game",         // タイトルバーの文字
        WS_OVERLAPPEDWINDOW,        // 標準的なウィンドウスタイル
        CW_USEDEFAULT,              // 表示X座標（OSに任せる）
        CW_USEDEFAULT,              // 表示Y座標（OSに任せる）
        wrc.right - wrc.left,       // ウィンドウ横幅
        wrc.bottom - wrc.top,   // ウィンドウ縦幅
        nullptr,                // 親ウィンドウハンドル
        nullptr,                // メニューハンドル
        w.hInstance,            // 呼び出しアプリケーションハンドル
        nullptr);               // オプション

    // ウィンドウ表示
    ShowWindow(hwnd, SW_SHOW);

#pragma endregion ウィンドウ生成
    //-------------

    MSG msg{}; //メッセージここから

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


    //基本変数初期化-----
#pragma region 基本変数初期化

    HRESULT result;
    ComPtr<ID3D12Device> device = nullptr;
    ComPtr<IDXGIFactory6> dxgiFactory = nullptr;
    ComPtr<IDXGISwapChain4> swapchain = nullptr;
    ComPtr<ID3D12CommandAllocator> commandAllocator = nullptr;
    ComPtr<ID3D12GraphicsCommandList> cmdList = nullptr;
    ComPtr<ID3D12CommandQueue> commandQueue = nullptr;
    ComPtr<ID3D12DescriptorHeap> rtvHeaps = nullptr;

#pragma endregion 基本変数初期化
    //--------------

       //DXGIファクトリー生成----------------
#pragma region DXGIファクトリー生成

//DXGIファクトリーの生成
    result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
    assert(SUCCEEDED(result));

#pragma endregion DXGIファクトリー生成
    //-------------------

    //アダプター設定ここから--------------------------------------------------------------------------------------------
#pragma region アダプター設定

    //アダプターの列挙用
    vector<ComPtr<IDXGIAdapter4>> adapters;
    //ここに特定の名前を持つアダプターオブジェクトが入る
    ComPtr <IDXGIAdapter4> tmpAdapter = nullptr;

    for (UINT i = 0; dxgiFactory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND; i++)
    {
        adapters.push_back(tmpAdapter);
    }

    for (size_t i = 0; i < adapters.size(); i++)
    {
        DXGI_ADAPTER_DESC3 adapterDesc;
        adapters[i]->GetDesc3(&adapterDesc);

        if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
        {
            tmpAdapter = adapters[i];
            break;
        }
    }

#pragma endregion アダプター設定
    //アダプター設定ここまで-----------------------------------------------------------------------

    //デバイス生成ここから----------------------------------------------------------------------------------------------------------
#pragma region デバイス生成

    //対応レベルの配列
    D3D_FEATURE_LEVEL levels[] =
    {
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };

    D3D_FEATURE_LEVEL featureLevel;

    for (int i = 0; i < _countof(levels); i++)
    {
        //採用したアダプターでデバイスを生成
        result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&device));
        if (result == S_OK)
        {
            featureLevel = levels[i];
            break;
        }
    }

#pragma endregion デバイス生成
    //デバイス生成ここまで----------------------------------------------------------------------

     //コマンドリストここから--------------------------------------------------------------------------------------------------------
#pragma region コマンドリスト

    //コマンドアロケータを生成
    result = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
    assert(SUCCEEDED(result));

    //コマンドリストを生成
    result = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&cmdList));
    assert(SUCCEEDED(result));


    //コマンドキューの生成
    //標準設定でコマンドキューを生成
    D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
    result = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
    assert(SUCCEEDED(result));

#pragma endregion コマンドリスト
    //コマンドリストここまで------------------------------------------------------------------------------------------


    //スワップチェーンの生成ここから------------------------------------------------------------------
#pragma region スワップチェーンの生成
    ComPtr<IDXGISwapChain1> swapchain1;

    //各種設定
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
    swapChainDesc.Width = window_width;
    swapChainDesc.Height = window_height;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

     result = dxgiFactory->CreateSwapChainForHwnd(commandQueue.Get(), hwnd, &swapChainDesc, nullptr, nullptr, &swapchain1);

     assert(SUCCEEDED(result));

    result = swapchain1.As(&swapchain);
     assert(SUCCEEDED(result));



#pragma endregion スワップチェーンの生成
    //スワップチェーンの生成ここまで--------------------------------------------------------------

     //レンダーターゲットビュー生成関係ここから---------------------------------------------------------------------------------------------------
#pragma region レンダーターゲットビュー生成関係

    //デスクリプタヒープを生成ここから--------------
#pragma region デスクリプタヒープ生成

//各種設定
     D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
     rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
     rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount;
     device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeaps));

#pragma endregion デスクリプタヒープ生成
     //デスクリプタヒープを生成ここまで------------

     //ダブルバッファ----------------------
#pragma region ダブルバッファ

     vector<ComPtr<ID3D12Resource>> backBuffers(2);

     backBuffers.resize(swapChainDesc.BufferCount);

     for (size_t i = 0; i < backBuffers.size(); i++)
     {
         //スワップチェーンからバッファを取得
         swapchain->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers[i]));
         //でスクリプタヒープハンドルを取得
         D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
         //裏か表かでアドレスがずれる
         rtvHandle.ptr += i * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
         //レンダーターゲットビューの設定
         D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
         //シェーダーの計算結果をSRGBに変換して書き込む
         rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
         rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
         //レンダーターゲットビューの生成
         device->CreateRenderTargetView(backBuffers[i].Get(), &rtvDesc, rtvHandle);

     }

#pragma endregion ダブルバッファ
     //------------------

#pragma endregion レンダーターゲットビュー生成関係



         //フェンスの生成---------
#pragma region フェンスの生成

     ComPtr<ID3D12Fence> fence = nullptr;
     UINT64 fenceVal = 0;

     result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

#pragma endregion フェンスの生成
     //------------
#pragma endregion 

    while (true)
    {

        //メッセージ関係------------------
#pragma region メッセージ関係

// メッセージがある？
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) 
        {
            TranslateMessage(&msg); // キー入力メッセージの処理
            DispatchMessage(&msg); // プロシージャにメッセージを送る
        }

        // xボタンで終了メッセージが来たらゲームループを抜ける
        if (msg.message == WM_QUIT) 
        {
            break;
        }

#pragma endregion メッセージ関係
        //--------------------
    }



    UnregisterClass(w.lpszClassName, w.hInstance);

	return 0;
}
