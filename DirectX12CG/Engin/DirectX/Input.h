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
        };
        static Input* GetInstance();
        static void DeleteInstace();
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
        bool IsKeyDown(int keyNum);
        //押していない状態
        bool IsKeyNDown(int keyNum);
        //長押し状態
        bool IsKeyPress(int keyNum);
        //押した瞬間
        bool IsKeyTrigger(int keyNum);
        //離した瞬間
        bool IsKeyRelease(int keyNum);

        //押している状態
        bool IsMouseDown(int MouseBotton);
        //押していない状態
        bool IsMouseNDown(int MouseBotton);
        //長押し状態
        bool IsMousePress(int MouseBotton);
        //押した瞬間
        bool IsMouseTrigger(int MouseBotton);
        //離した瞬間
        bool IsMouseRelease(int MouseBotton);


        MCB::Mouse GetMousePosition();
#pragma endregion 入力関数
        //--------------------

        void Init();

        void UpDateInit();


    };
}


