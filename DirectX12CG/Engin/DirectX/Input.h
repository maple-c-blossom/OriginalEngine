#pragma once
#include <dinput.h>
#include "Dx12.h"
#include "DxWindow.h"
#include "GamePad.h"
#define DIRECINPUT_VERSION 0x0800 //DirectInput�̃o�[�W�����w��

namespace MCB
{
    
    struct Mouse
    {
        LONG x;
        LONG y;
        LONG z;
    };
    class Input
    {
        
    private:
        Microsoft::WRL::ComPtr<IDirectInputDevice8> devkeyboard = nullptr;
        Microsoft::WRL::ComPtr <IDirectInputDevice8> devmouse = nullptr;

        Microsoft::WRL::ComPtr <IDirectInput8> dinput = nullptr;
        Input() {};
        Input(const Input& input) {};
        ~Input() {};
        Input& operator= (const Input & Input){};

    public:
        typedef enum MouseBotton
        {
            LEFT = 0,
            RIGHT = 1,
            MIDDLE = 2
        }MouseBotton;
        static Input* GetInstance();
        static void AllStopVibration();
        static Input* GetInitInstance();
        GamePad* gamePad = GamePad::GetInitInstance();

        //�L�[������----------------------
        BYTE key[256] = {};
        BYTE oldkey[256] = {};
        //-----------------------
        DIMOUSESTATE2 mouse = {};
        DIMOUSESTATE2 oldmouse = {};
        //Input(HRESULT& result, WNDCLASSEX w, HWND hwnd);

        //�L�[�{�[�h������-------------------------------
        void KeyUpdate();
        //---------------------

        void MouseUpdate();

        //���͊֐�-----------------------------------
#pragma region ���͊֐�
//�����Ă�����
        bool IsKeyDown(const size_t& keyNum);
        //�����Ă��Ȃ����
        bool IsKeyNDown(const size_t& keyNum);
        //���������
        bool IsKeyPress(const size_t& keyNum);
        //�������u��
        bool IsKeyTrigger(const size_t& keyNum);
        //�������u��
        bool IsKeyRelease(const size_t& keyNum);

        //�����Ă�����
        bool IsMouseDown(const size_t& MouseBotton);
        //�����Ă��Ȃ����
        bool IsMouseNDown(const size_t& MouseBotton);
        //���������
        bool IsMousePress(const size_t& MouseBotton);
        //�������u��
        bool IsMouseTrigger(const size_t& MouseBotton);
        //�������u��
        bool IsMouseRelease(const size_t& MouseBotton);


        MCB::Mouse GetMousePosition();
#pragma endregion ���͊֐�
        //--------------------

        void Init();

        void UpDateInit();


    };
}


