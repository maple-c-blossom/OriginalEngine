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

	dx12->result_ = DirectInput8Create(dxWindow->window_.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&dinput_, nullptr);
	assert(SUCCEEDED(dx12->result_));
	//キーボードデバイスの生成-----------------
#pragma region キーボードデバイスの生成
	dx12->result_ = dinput_->CreateDevice(GUID_SysKeyboard, &devkeyboard_, NULL);
	assert(SUCCEEDED(dx12->result_));
#pragma endregion キーボードデバイスの生成

	//入力データ形式セット--------------------------------
	dx12->result_ = devkeyboard_->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(dx12->result_));
	//---------------------------------

	//排他レベル制御-------------------------------------------------------------
	dx12->result_ = devkeyboard_->SetCooperativeLevel(dxWindow->hwnd_,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(dx12->result_));
	//--------------------------

	//マウスデバイスの生成-----------------
#pragma region マウスデバイスの生成
	dx12->result_ = dinput_->CreateDevice(GUID_SysMouse, &devmouse_, NULL);
	assert(SUCCEEDED(dx12->result_));
#pragma endregion マウスデバイスの生成
	//--------------------------
	// 入力データ形式のセット
	dx12->result_ = devmouse_->SetDataFormat(&c_dfDIMouse2); // 標準形式
	assert(SUCCEEDED(dx12->result_));
	// 排他制御レベルのセット
	dx12->result_ = devmouse_->SetCooperativeLevel(
		dxWindow->hwnd_, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(dx12->result_));

#pragma endregion 入力系初期化
	//----------------
}

void Input::UpDateInit()
{
	
	//キーボード初期化-------------------------------------
#pragma region キーボード初期化

	//キーボード情報の取得開始-----------------
	devkeyboard_->Acquire();
	//----------------------------
	//全キーの入力状態を取得する---------------------------
	KeyUpdate();
	Dx12::GetInstance()->result_ = devkeyboard_->GetDeviceState(sizeof(key_), key_);
	//----------------------------

	// マウス
	devmouse_->Acquire();	// マウス動作開始
	MouseUpdate();
	// マウスの入力
	Dx12::GetInstance()->result_ = devmouse_->GetDeviceState(sizeof(mouse_), &mouse_);
#pragma endregion キーボード初期化
		//----------------------------------------
	gamePad_->GetState();
	gamePad_->LStick_ = gamePad_->IsInputLStick();
	gamePad_->RStick_ = gamePad_->IsInputRStick();
	gamePad_->LTrriger_ = gamePad_->IsInputLTrriger();
	gamePad_->RTrriger_ = gamePad_->IsInputRTrriger();
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
	static Input instance ;
	return &instance;
}

void MCB::Input::AllStopVibration()
{
	GamePad::AllStopVibration();
	//delete Input::GetInstance();
}

Input* MCB::Input::GetInitInstance()
{
	static Input* instance = Input::GetInstance();
	instance->Init();
	return instance;
}

void Input::KeyUpdate()
{
	for (size_t i = 0; i < 256; i++)
	{
		oldkey_[i] = key_[i];
	}

}

void MCB::Input::MouseUpdate()
{
	oldmouse_ = mouse_;
}

bool Input::IsKeyDown( size_t keyNum)
{
		if (key_[keyNum])
		{
			return true;
		}
		return false;
}

bool Input::IsKeyNDown( size_t keyNum)
{
	if (!key_[keyNum])
	{
		return true;
	}
	return false;
}

bool Input::IsKeyPress( size_t keyNum)
{
	if (key_[keyNum] && oldkey_[keyNum])
	{
		return true;
	}
	return false;
}

bool Input::IsKeyTrigger( size_t keyNum)
{
	if (key_[keyNum] && !oldkey_[keyNum])
	{
		return true;
	}
	return false;
}

bool Input::IsKeyRelease( size_t keyNum)
{
	if (!key_[keyNum] && oldkey_[keyNum])
	{
		return true;
	}
	return false;
}

bool MCB::Input::IsMouseDown( size_t MouseBotton)
{
	if (mouse_.rgbButtons[MouseBotton]) return true;
	return false;
}

bool MCB::Input::IsMouseNDown( size_t MouseBotton)
{
	if (!mouse_.rgbButtons[MouseBotton]) return true;
	return false;
}

bool MCB::Input::IsMousePress( size_t MouseBotton)
{
	if (mouse_.rgbButtons[MouseBotton] && oldmouse_.rgbButtons[MouseBotton]) return true;
	return false;
}

bool MCB::Input::IsMouseTrigger( size_t MouseBotton)
{
	if (mouse_.rgbButtons[MouseBotton] && !oldmouse_.rgbButtons[MouseBotton]) return true;
	return false;
}

bool MCB::Input::IsMouseRelease( size_t MouseBotton)
{
	if (!mouse_.rgbButtons[MouseBotton] && oldmouse_.rgbButtons[MouseBotton])
	{
		return true;
	}
	return false;
}

MCB::Mouse MCB::Input::GetMousePosition()
{
	MCB::Mouse m;
	m.x = mouse_.lX;
	m.y = mouse_.lY;
	m.z = mouse_.lZ;
	return m;
}

