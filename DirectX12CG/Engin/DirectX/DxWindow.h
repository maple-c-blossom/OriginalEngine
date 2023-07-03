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

        // �E�B���h�E�T�C�Y
        static const int32_t sWINDOW_WIDTH_ = 1280;  // ����
        static const int32_t sWINDOW_HEIGHT_ = 720;  // �c��


        MSG msg_{}; //���b�Z�[�W��������
        WNDCLASSEX window_{}; // �E�B���h�E�N���X�̐ݒ�
            // �E�B���h�E�T�C�Y{ X���W Y���W ���� �c�� }
        RECT wrc = { 0, 0, sWINDOW_WIDTH_, sWINDOW_HEIGHT_ };
        HWND hwnd_;
        const wchar_t* windowName = L"MapleEngin";


        // �E�B���h�E�v���V�[�W�� 
        static LRESULT WindowProc( HWND hwnd,  uint32_t msg,  WPARAM wparam,  LPARAM lparam);
        bool IsBreak();
        void messageUpdate();
        void Init();

    };
}


