#include <Windows.h> 



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
