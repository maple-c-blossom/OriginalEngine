#pragma once
#include <dinput.h>
#define DIRECINPUT_VERSION 0x0800 //DirectInput�̃o�[�W�����w��

namespace MCB
{
    class Input
    {
    public:
        IDirectInputDevice8* devkeyboard = nullptr;

        IDirectInput8* dinput = nullptr;

        //�L�[������----------------------
        BYTE key[256] = {};
        BYTE oldkey[256] = {};
        //-----------------------

        Input(HRESULT& result, WNDCLASSEX w, HWND hwnd);

        //�L�[�{�[�h������-------------------------------
        void KeyInit();
        //---------------------

        //���͊֐�-----------------------------------
#pragma region ���͊֐�
//�����Ă�����
        bool IsKeyDown(int keyNum);
        //�����Ă��Ȃ����
        bool IsKeyNDown(int keyNum);
        //���������
        bool IsKeyPress(int keyNum);
        //�������u��
        bool IsKeyTrigger(int keyNum);
        //�������u��
        bool IsKeyRelease(int keyNum);
#pragma endregion ���͊֐�
        //--------------------

        void Init(HRESULT& result, WNDCLASSEX w, HWND hwnd);

        void UpDateInit(HRESULT& result);
    };
}

