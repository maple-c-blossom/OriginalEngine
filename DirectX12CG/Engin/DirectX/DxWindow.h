#pragma once
#include <Windows.h>
#include <cstdint>

namespace MCB
{
    class DxWindow
    {
    private:
        DxWindow() {};
        DxWindow(const DxWindow& dx12) {};
        DxWindow& operator=(const DxWindow& dx12) {};
        ~DxWindow();
        bool breakFlag_ = false;

    public:
        static DxWindow* GetInstance();
        static DxWindow* GetInitInstance();
        static void DeleteInstance();

        // ウィンドウサイズ
        static const int32_t sWINDOW_WIDTH_ = 1280;  // 横幅
        static const int32_t sWINDOW_HEIGHT_ = 720;  // 縦幅


        MSG msg_{}; //メッセージここから
        WNDCLASSEX window_{}; // ウィンドウクラスの設定
            // ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
        RECT wrc = { 0, 0, sWINDOW_WIDTH_, sWINDOW_HEIGHT_ };
        HWND hwnd_;
        const wchar_t* windowName = L"MapleEngin";


        // ウィンドウプロシージャ 
        static LRESULT WindowProc( HWND hwnd,  uint32_t msg,  WPARAM wparam,  LPARAM lparam);
        bool IsBreak();
        void messageUpdate();
        void Init();

    };
}


