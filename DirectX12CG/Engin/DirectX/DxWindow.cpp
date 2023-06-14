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

bool MCB::DxWindow::IsBreak()
{
    return breakFlag_;
}

void DxWindow::messageUpdate()
{
    //���b�Z�[�W�֌W------------------
#pragma region ���b�Z�[�W�֌W

// ���b�Z�[�W������H
    if (PeekMessage(&msg_, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg_); // �L�[���̓��b�Z�[�W�̏���
        DispatchMessage(&msg_); // �v���V�[�W���Ƀ��b�Z�[�W�𑗂�
    }

    // x�{�^���ŏI�����b�Z�[�W��������Q�[�����[�v�𔲂���
    if (msg_.message == WM_QUIT)
    {
        breakFlag_ = true;
    }

#pragma endregion ���b�Z�[�W�֌W
    //--------------------
}

void DxWindow::Init()
{
    timeBeginPeriod(1);
    window_.cbSize = sizeof(WNDCLASSEX);
    window_.lpfnWndProc = (WNDPROC)WindowProc; // �E�B���h�E�v���V�[�W����ݒ�
    window_.lpszClassName = windowName; // �E�B���h�E�N���X��
    window_.hInstance = GetModuleHandle(nullptr); // �E�B���h�E�n���h��
    window_.hCursor = LoadCursor(NULL, IDC_ARROW); // �J�[�\���w��
    window_.hIcon = LoadIcon(window_.hInstance, MAKEINTRESOURCE(MapleIcon));
    window_.hIconSm = LoadIcon(window_.hInstance, MAKEINTRESOURCE(MapleIcon));
    // �E�B���h�E�N���X��OS�ɓo�^
    RegisterClassEx(&window_);

    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); // �����ŃT�C�Y�␳


    // �E�B���h�E�I�u�W�F�N�g�̐���
    hwnd_ = CreateWindow(window_.lpszClassName, // �N���X��
        windowName,         // �^�C�g���o�[�̕���
        WS_OVERLAPPEDWINDOW,        // �W���I�ȃE�B���h�E�X�^�C��
        CW_USEDEFAULT,              // �\��X���W�iOS�ɔC����j
        CW_USEDEFAULT,              // �\��Y���W�iOS�ɔC����j
        wrc.right - wrc.left,       // �E�B���h�E����
        wrc.bottom - wrc.top,   // �E�B���h�E�c��
        nullptr,                // �e�E�B���h�E�n���h��
        nullptr,                // ���j���[�n���h��
        window_.hInstance,            // �Ăяo���A�v���P�[�V�����n���h��
        nullptr);               // �I�v�V����

    // �E�B���h�E�\��
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

