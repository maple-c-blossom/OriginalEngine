#include "DxWindow.h"
#include <Windows.h>
LRESULT DxWindow::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
     // メッセージに応じてゲーム固有の処理を行う 
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

void DxWindow::messageUpdate()
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
        breakFlag = true;
    }

#pragma endregion メッセージ関係
    //--------------------
}

DxWindow::DxWindow()
{

    w.cbSize = sizeof(WNDCLASSEX);
    w.lpfnWndProc = (WNDPROC)WindowProc; // ウィンドウプロシージャを設定
    w.lpszClassName = L"DirectXGame"; // ウィンドウクラス名
    w.hInstance = GetModuleHandle(nullptr); // ウィンドウハンドル
    w.hCursor = LoadCursor(NULL, IDC_ARROW); // カーソル指定

    // ウィンドウクラスをOSに登録
    RegisterClassEx(&w);

    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); // 自動でサイズ補正


    // ウィンドウオブジェクトの生成
    hwnd = CreateWindow(w.lpszClassName, // クラス名
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
}

DxWindow::~DxWindow()
{
    UnregisterClass(w.lpszClassName, w.hInstance);
}

