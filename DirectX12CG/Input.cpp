#include "Input.h"
#include <cassert>
#define DIRECINPUT_VERSION 0x0800 //DirectInputのバージョン指定

using namespace MCB;

void Input::Init()
{
	Dx12* dx12 = Dx12::GetInstance();
	DxWindow* dxWindow = DxWindow::GetInstance();
	//入力系初期化--------------
#pragma region 入力系初期化


	dx12->result = DirectInput8Create(dxWindow->window.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
	assert(SUCCEEDED(dx12->result));
	//キーボードデバイスの生成-----------------
#pragma region キーボードデバイスの生成


	dx12->result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
	assert(SUCCEEDED(dx12->result));

#pragma endregion キーボードデバイスの生成
	//--------------------------

	//入力データ形式セット--------------------------------
	dx12->result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(dx12->result));
	//---------------------------------

	//排他レベル制御-------------------------------------------------------------
	dx12->result = devkeyboard->SetCooperativeLevel(dxWindow->hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(dx12->result));

#pragma endregion 入力系初期化
	//----------------
}

void Input::UpDateInit()
{
	
	//キーボード初期化-------------------------------------
#pragma region キーボード初期化

		//キーボード情報の取得開始-----------------
	devkeyboard->Acquire();
	//----------------------------

	//全キーの入力状態を取得する---------------------------
	KeyInit();
	Dx12::GetInstance()->result = devkeyboard->GetDeviceState(sizeof(key), key);
	//----------------------------

#pragma endregion キーボード初期化
		//----------------------------------------

}

//Input::Input(HRESULT& result, WNDCLASSEX w, HWND hwnd)
//{
//	//入力系初期化--------------
//#pragma region 入力系初期化
//
//
//	result = DirectInput8Create(w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
//	assert(SUCCEEDED(result));
//	//キーボードデバイスの生成-----------------
//#pragma region キーボードデバイスの生成
//
//
//	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
//	assert(SUCCEEDED(result));
//
//#pragma endregion キーボードデバイスの生成
//	//--------------------------
//
//	//入力データ形式セット--------------------------------
//	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);
//	assert(SUCCEEDED(result));
//	//---------------------------------
//
//	//排他レベル制御-------------------------------------------------------------
//	result = devkeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
//	assert(SUCCEEDED(result));
//
//#pragma endregion 入力系初期化
//	//----------------
//}


Input* MCB::Input::GetInstance()
{
	static Input* instance = new Input;
	return instance;
}

void MCB::Input::DeleteInstace()
{
	delete Input::GetInstance();
}

Input* MCB::Input::GetInitInstance()
{
	static Input* instance = Input::GetInstance();
	instance->Init();
	return instance;
}

void Input::KeyInit()
{
	for (int i = 0; i < 256; i++)
	{
		oldkey[i] = key[i];
	}

}

bool Input::IsKeyDown(int keyNum)
{
		if (key[keyNum])
		{
			return true;
		}
		return false;
}

bool Input::IsKeyNDown(int keyNum)
{
	if (!key[keyNum])
	{
		return true;
	}
	return false;
}

bool Input::IsKeyPress(int keyNum)
{
	if (key[keyNum] && oldkey[keyNum])
	{
		return true;
	}
	return false;
}

bool Input::IsKeyTrigger(int keyNum)
{
	if (key[keyNum] && !oldkey[keyNum])
	{
		return true;
	}
	return false;
}

bool Input::IsKeyRelease(int keyNum)
{
	if (!key[keyNum] && oldkey[keyNum])
	{
		return true;
	}
	return false;
}

