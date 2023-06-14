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
        int64_t x;
        int64_t y;
        int64_t z;
    };
    class Input
    {
        
    private:
        Microsoft::WRL::ComPtr<IDirectInputDevice8> devkeyboard_ = nullptr;
        Microsoft::WRL::ComPtr <IDirectInputDevice8> devmouse_ = nullptr;

        Microsoft::WRL::ComPtr <IDirectInput8> dinput_ = nullptr;
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
        GamePad* gamePad_ = GamePad::GetInitInstance();

        //�L�[������----------------------
        BYTE key_[256] = {};
        BYTE oldkey_[256] = {};
        //-----------------------
        DIMOUSESTATE2 mouse_ = {};
        DIMOUSESTATE2 oldmouse_ = {};
        //Input(HRESULT& result, WNDCLASSEX w, HWND hwnd);

        //�L�[�{�[�h������-------------------------------
        void KeyUpdate();
        //---------------------

        void MouseUpdate();

        //���͊֐�-----------------------------------
#pragma region ���͊֐�
//�����Ă�����
        bool IsKeyDown( size_t keyNum);
        //�����Ă��Ȃ����
        bool IsKeyNDown( size_t keyNum);
        //���������
        bool IsKeyPress( size_t keyNum);
        //�������u��
        bool IsKeyTrigger( size_t keyNum);
        //�������u��
        bool IsKeyRelease( size_t keyNum);

        //�����Ă�����
        bool IsMouseDown( size_t MouseBotton);
        //�����Ă��Ȃ����
        bool IsMouseNDown( size_t MouseBotton);
        //���������
        bool IsMousePress( size_t MouseBotton);
        //�������u��
        bool IsMouseTrigger( size_t MouseBotton);
        //�������u��
        bool IsMouseRelease( size_t MouseBotton);


        MCB::Mouse GetMousePosition();
#pragma endregion ���͊֐�
        //--------------------

        void Init();

        void UpDateInit();


    };
}


