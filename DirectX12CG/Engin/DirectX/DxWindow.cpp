#include "DxWindow.h"
#include "resource.h"
#include <MyImgui/imgui_impl_win32.h>
#pragma comment(lib, "winmm.lib")
using namespace MCB;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lparam);
LRESULT DxWindow::WindowProc( HWND hwnd, uint32_t msg, WPARAM wparam, LPARAM lparam)
{
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
    {
        return true;
    }
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

bool MCB::DxWindow::IsBreak()
{
    return breakFlag_;
}

void DxWindow::messageUpdate()
{
    //メッセージ関係------------------
#pragma region メッセージ関係

// メッセージがある？
    if (PeekMessage(&msg_, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg_); // キー入力メッセージの処理
        DispatchMessage(&msg_); // プロシージャにメッセージを送る
    }

    // xボタンで終了メッセージが来たらゲームループを抜ける
    if (msg_.message == WM_QUIT)
    {
        breakFlag_ = true;
    }

#pragma endregion メッセージ関係
    //--------------------
}

void DxWindow::Init()
{
    timeBeginPeriod(1);
    window_.cbSize = sizeof(WNDCLASSEX);
    window_.lpfnWndProc = (WNDPROC)WindowProc; // ウィンドウプロシージャを設定
    window_.lpszClassName = windowName; // ウィンドウクラス名
    window_.hInstance = GetModuleHandle(nullptr); // ウィンドウハンドル
    window_.hCursor = LoadCursor(NULL, IDC_ARROW); // カーソル指定
    window_.hIcon = LoadIcon(window_.hInstance, MAKEINTRESOURCE(MapleIcon));
    window_.hIconSm = LoadIcon(window_.hInstance, MAKEINTRESOURCE(MapleIcon));
    // ウィンドウクラスをOSに登録
    RegisterClassEx(&window_);

    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); // 自動でサイズ補正


    // ウィンドウオブジェクトの生成
    hwnd_ = CreateWindow(window_.lpszClassName, // クラス名
        windowName,         // タイトルバーの文字
        WS_OVERLAPPEDWINDOW,        // 標準的なウィンドウスタイル
        CW_USEDEFAULT,              // 表示X座標（OSに任せる）
        CW_USEDEFAULT,              // 表示Y座標（OSに任せる）
        wrc.right - wrc.left,       // ウィンドウ横幅
        wrc.bottom - wrc.top,   // ウィンドウ縦幅
        nullptr,                // 親ウィンドウハンドル
        nullptr,                // メニューハンドル
        window_.hInstance,            // 呼び出しアプリケーションハンドル
        nullptr);               // オプション

    // ウィンドウ表示
    ShowWindow(hwnd_, SW_SHOW);
}

DxWindow::~DxWindow()
{
    UnregisterClass(window_.lpszClassName, window_.hInstance);
}

DxWindow* MCB::DxWindow::GetInstance()
{
    static DxWindow instance;
    return &instance;
}

DxWindow* MCB::DxWindow::GetInitInstance()
{
    static DxWindow* instance = GetInstance();
    instance->Init();
    return instance;
}

void MCB::DxWindow::DeleteInstance()
{
    //delete DxWindow::GetInstance();
}

