#pragma once
#include <dinput.h>
#include "Dx12.h"
#include "DxWindow.h"
#include "GamePad.h"
#define DIRECINPUT_VERSION 0x0800 //DirectInputのバージョン指定

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

        //キー初期化----------------------
        BYTE key[256] = {};
        BYTE oldkey[256] = {};
        //-----------------------
        DIMOUSESTATE2 mouse = {};
        DIMOUSESTATE2 oldmouse = {};
        //Input(HRESULT& result, WNDCLASSEX w, HWND hwnd);

        //キーボード初期化-------------------------------
        void KeyUpdate();
        //---------------------

        void MouseUpdate();

        //入力関数-----------------------------------
#pragma region 入力関数
//押している状態
        bool IsKeyDown(size_t keyNum);
        //押していない状態
        bool IsKeyNDown(size_t keyNum);
        //長押し状態
        bool IsKeyPress(size_t keyNum);
        //押した瞬間
        bool IsKeyTrigger(size_t keyNum);
        //離した瞬間
        bool IsKeyRelease(size_t keyNum);

        //押している状態
        bool IsMouseDown(size_t MouseBotton);
        //押していない状態
        bool IsMouseNDown(size_t MouseBotton);
        //長押し状態
        bool IsMousePress(size_t MouseBotton);
        //押した瞬間
        bool IsMouseTrigger(size_t MouseBotton);
        //離した瞬間
        bool IsMouseRelease(size_t MouseBotton);


        MCB::Mouse GetMousePosition();
#pragma endregion 入力関数
        //--------------------

        void Init();

        void UpDateInit();


    };
}


