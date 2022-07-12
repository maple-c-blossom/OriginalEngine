#pragma once
#include <dinput.h>
#define DIRECINPUT_VERSION 0x0800 //DirectInputのバージョン指定

namespace MCB
{
    class Input
    {
    private:
        IDirectInputDevice8* devkeyboard = nullptr;

        IDirectInput8* dinput = nullptr;

        Input() {};
        Input(const Input& input) {};
        ~Input() {};
        Input& operator= (const Input & Input){};

    public:

        static Input* GetInstance();
        static void DeleteInstace();
        static Input* GetInitInstance(HRESULT& result, WNDCLASSEX w, HWND hwnd);

        //キー初期化----------------------
        BYTE key[256] = {};
        BYTE oldkey[256] = {};
        //-----------------------

        //Input(HRESULT& result, WNDCLASSEX w, HWND hwnd);

        //キーボード初期化-------------------------------
        void KeyInit();
        //---------------------

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
#pragma endregion 入力関数
        //--------------------

        void Init(HRESULT& result, WNDCLASSEX w, HWND hwnd);

        void UpDateInit(HRESULT& result);
    };
}


