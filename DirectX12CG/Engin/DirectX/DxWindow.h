#pragma once
#include <Windows.h>

namespace MCB
{
    class DxWindow
    {
    private:
        DxWindow() {};
        DxWindow(const DxWindow& dx12) {};
        DxWindow& operator=(const DxWindow& dx12) {};
        ~DxWindow();
        bool breakFlag = false;

    public:
        static DxWindow* GetInstance();
        static DxWindow* GetInitInstance();
        static void DeleteInstance();

        // ウィンドウサイズ
        static const int32_t window_width = 1280;  // 横幅
        static const int32_t window_height = 720;  // 縦幅


        MSG msg{}; //メッセージここから
        WNDCLASSEX window{}; // ウィンドウクラスの設定
            // ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
        RECT wrc = { 0, 0, window_width, window_height };
        HWND hwnd;
        const wchar_t* windowName = L"MapleEngin";


        // ウィンドウプロシージャ 
        static LRESULT WindowProc(const HWND& hwnd,const  uint32_t& msg, const WPARAM& wparam, const LPARAM& lparam);
        bool IsBreak();
        void messageUpdate();
        void Init();

    };
}


