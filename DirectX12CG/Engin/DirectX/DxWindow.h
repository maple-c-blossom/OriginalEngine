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

        // �E�B���h�E�T�C�Y
        static const int32_t window_width = 1280;  // ����
        static const int32_t window_height = 720;  // �c��


        MSG msg{}; //���b�Z�[�W��������
        WNDCLASSEX window{}; // �E�B���h�E�N���X�̐ݒ�
            // �E�B���h�E�T�C�Y{ X���W Y���W ���� �c�� }
        RECT wrc = { 0, 0, window_width, window_height };
        HWND hwnd;
        const wchar_t* windowName = L"MapleEngin";


        // �E�B���h�E�v���V�[�W�� 
        static LRESULT WindowProc(const HWND& hwnd,const  uint32_t& msg, const WPARAM& wparam, const LPARAM& lparam);
        bool IsBreak();
        void messageUpdate();
        void Init();

    };
}


