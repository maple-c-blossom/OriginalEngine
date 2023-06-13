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

        //キー初期化----------------------
        BYTE key_[256] = {};
        BYTE oldkey_[256] = {};
        //-----------------------
        DIMOUSESTATE2 mouse_ = {};
        DIMOUSESTATE2 oldmouse_ = {};
        //Input(HRESULT& result, WNDCLASSEX w, HWND hwnd);

        //キーボード初期化-------------------------------
        void KeyUpdate();
        //---------------------

        void MouseUpdate();

        //入力関数-----------------------------------
#pragma region 入力関数
//押している状態
        bool IsKeyDown(const size_t& keyNum);
        //押していない状態
        bool IsKeyNDown(const size_t& keyNum);
        //長押し状態
        bool IsKeyPress(const size_t& keyNum);
        //押した瞬間
        bool IsKeyTrigger(const size_t& keyNum);
        //離した瞬間
        bool IsKeyRelease(const size_t& keyNum);

        //押している状態
        bool IsMouseDown(const size_t& MouseBotton);
        //押していない状態
        bool IsMouseNDown(const size_t& MouseBotton);
        //長押し状態
        bool IsMousePress(const size_t& MouseBotton);
        //押した瞬間
        bool IsMouseTrigger(const size_t& MouseBotton);
        //離した瞬間
        bool IsMouseRelease(const size_t& MouseBotton);


        MCB::Mouse GetMousePosition();
#pragma endregion 入力関数
        //--------------------

        void Init();

        void UpDateInit();


    };
}


