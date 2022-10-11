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
        };
        static Input* GetInstance();
        static void DeleteInstace();
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
        bool IsKeyDown(int keyNum);
        //�����Ă��Ȃ����
        bool IsKeyNDown(int keyNum);
        //���������
        bool IsKeyPress(int keyNum);
        //�������u��
        bool IsKeyTrigger(int keyNum);
        //�������u��
        bool IsKeyRelease(int keyNum);

        //�����Ă�����
        bool IsMouseDown(int MouseBotton);
        //�����Ă��Ȃ����
        bool IsMouseNDown(int MouseBotton);
        //���������
        bool IsMousePress(int MouseBotton);
        //�������u��
        bool IsMouseTrigger(int MouseBotton);
        //�������u��
        bool IsMouseRelease(int MouseBotton);


        MCB::Mouse GetMousePosition();
#pragma endregion ���͊֐�
        //--------------------

        void Init();

        void UpDateInit();


    };
}


