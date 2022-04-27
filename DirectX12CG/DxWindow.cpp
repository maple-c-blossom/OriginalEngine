#include "DxWindow.h"
#include <Windows.h>

using namespace MCB;

LRESULT DxWindow::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
     // ���b�Z�[�W�ɉ����ăQ�[���ŗL�̏������s�� 
        switch (msg) {
            // �E�B���h�E���j�����ꂽ 
        case WM_DESTROY:
            // OS�ɑ΂��āA�A�v���̏I����`���� 
            PostQuitMessage(0);
            return 0;
        }
        // �W���̃��b�Z�[�W�������s�� 
        return DefWindowProc(hwnd, msg, wparam, lparam);
    
}

void DxWindow::messageUpdate()
{
    //���b�Z�[�W�֌W------------------
#pragma region ���b�Z�[�W�֌W

// ���b�Z�[�W������H
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg); // �L�[���̓��b�Z�[�W�̏���
        DispatchMessage(&msg); // �v���V�[�W���Ƀ��b�Z�[�W�𑗂�
    }

    // x�{�^���ŏI�����b�Z�[�W��������Q�[�����[�v�𔲂���
    if (msg.message == WM_QUIT)
    {
        breakFlag = true;
    }

#pragma endregion ���b�Z�[�W�֌W
    //--------------------
}

DxWindow::DxWindow()
{

    window.cbSize = sizeof(WNDCLASSEX);
    window.lpfnWndProc = (WNDPROC)WindowProc; // �E�B���h�E�v���V�[�W����ݒ�
    window.lpszClassName = L"DirectXGame"; // �E�B���h�E�N���X��
    window.hInstance = GetModuleHandle(nullptr); // �E�B���h�E�n���h��
    window.hCursor = LoadCursor(NULL, IDC_ARROW); // �J�[�\���w��

    // �E�B���h�E�N���X��OS�ɓo�^
    RegisterClassEx(&window);

    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); // �����ŃT�C�Y�␳


    // �E�B���h�E�I�u�W�F�N�g�̐���
    hwnd = CreateWindow(window.lpszClassName, // �N���X��
        L"DirectX12Game",         // �^�C�g���o�[�̕���
        WS_OVERLAPPEDWINDOW,        // �W���I�ȃE�B���h�E�X�^�C��
        CW_USEDEFAULT,              // �\��X���W�iOS�ɔC����j
        CW_USEDEFAULT,              // �\��Y���W�iOS�ɔC����j
        wrc.right - wrc.left,       // �E�B���h�E����
        wrc.bottom - wrc.top,   // �E�B���h�E�c��
        nullptr,                // �e�E�B���h�E�n���h��
        nullptr,                // ���j���[�n���h��
        window.hInstance,            // �Ăяo���A�v���P�[�V�����n���h��
        nullptr);               // �I�v�V����

    // �E�B���h�E�\��
    ShowWindow(hwnd, SW_SHOW);
}

DxWindow::~DxWindow()
{
    UnregisterClass(window.lpszClassName, window.hInstance);
}
