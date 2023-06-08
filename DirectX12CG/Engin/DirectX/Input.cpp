#include "Input.h"
#include <cassert>
#define DIRECINPUT_VERSION 0x0800 //DirectInput�̃o�[�W�����w��

using namespace MCB;

void Input::Init()
{
	Dx12* dx12 = Dx12::GetInstance();
	DxWindow* dxWindow = DxWindow::GetInstance();
	//���͌n������--------------
#pragma region ���͌n������

	dx12->result = DirectInput8Create(dxWindow->window.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
	assert(SUCCEEDED(dx12->result));
	//�L�[�{�[�h�f�o�C�X�̐���-----------------
#pragma region �L�[�{�[�h�f�o�C�X�̐���
	dx12->result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
	assert(SUCCEEDED(dx12->result));
#pragma endregion �L�[�{�[�h�f�o�C�X�̐���

	//���̓f�[�^�`���Z�b�g--------------------------------
	dx12->result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(dx12->result));
	//---------------------------------

	//�r�����x������-------------------------------------------------------------
	dx12->result = devkeyboard->SetCooperativeLevel(dxWindow->hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(dx12->result));
	//--------------------------

	//�}�E�X�f�o�C�X�̐���-----------------
#pragma region �}�E�X�f�o�C�X�̐���
	dx12->result = dinput->CreateDevice(GUID_SysMouse, &devmouse, NULL);
	assert(SUCCEEDED(dx12->result));
#pragma endregion �}�E�X�f�o�C�X�̐���
	//--------------------------
	// ���̓f�[�^�`���̃Z�b�g
	dx12->result = devmouse->SetDataFormat(&c_dfDIMouse2); // �W���`��
	assert(SUCCEEDED(dx12->result));
	// �r�����䃌�x���̃Z�b�g
	dx12->result = devmouse->SetCooperativeLevel(
		dxWindow->hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(dx12->result));

#pragma endregion ���͌n������
	//----------------
}

void Input::UpDateInit()
{
	
	//�L�[�{�[�h������-------------------------------------
#pragma region �L�[�{�[�h������

	//�L�[�{�[�h���̎擾�J�n-----------------
	devkeyboard->Acquire();
	//----------------------------
	//�S�L�[�̓��͏�Ԃ��擾����---------------------------
	KeyUpdate();
	Dx12::GetInstance()->result = devkeyboard->GetDeviceState(sizeof(key), key);
	//----------------------------

	// �}�E�X
	devmouse->Acquire();	// �}�E�X����J�n
	MouseUpdate();
	// �}�E�X�̓���
	Dx12::GetInstance()->result = devmouse->GetDeviceState(sizeof(mouse), &mouse);
#pragma endregion �L�[�{�[�h������
		//----------------------------------------
	gamePad->GetState();
	gamePad->LStick = gamePad->IsInputLStick();
	gamePad->RStick = gamePad->IsInputRStick();
	gamePad->LTrriger = gamePad->IsInputLTrriger();
	gamePad->RTrriger = gamePad->IsInputRTrriger();
}

//Input::Input(HRESULT& result, WNDCLASSEX w, HWND hwnd)
//{
//	//���͌n������--------------
//#pragma region ���͌n������
//
//
//	result = DirectInput8Create(w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
//	assert(SUCCEEDED(result));
//	//�L�[�{�[�h�f�o�C�X�̐���-----------------
//#pragma region �L�[�{�[�h�f�o�C�X�̐���
//
//
//	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
//	assert(SUCCEEDED(result));
//
//#pragma endregion �L�[�{�[�h�f�o�C�X�̐���
//	//--------------------------
//
//	//���̓f�[�^�`���Z�b�g--------------------------------
//	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);
//	assert(SUCCEEDED(result));
//	//---------------------------------
//
//	//�r�����x������-------------------------------------------------------------
//	result = devkeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
//	assert(SUCCEEDED(result));
//
//#pragma endregion ���͌n������
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
		oldkey[i] = key[i];
	}

}

void MCB::Input::MouseUpdate()
{
	oldmouse = mouse;
}

bool Input::IsKeyDown(size_t keyNum)
{
		if (key[keyNum])
		{
			return true;
		}
		return false;
}

bool Input::IsKeyNDown(size_t keyNum)
{
	if (!key[keyNum])
	{
		return true;
	}
	return false;
}

bool Input::IsKeyPress(size_t keyNum)
{
	if (key[keyNum] && oldkey[keyNum])
	{
		return true;
	}
	return false;
}

bool Input::IsKeyTrigger(size_t keyNum)
{
	if (key[keyNum] && !oldkey[keyNum])
	{
		return true;
	}
	return false;
}

bool Input::IsKeyRelease(size_t keyNum)
{
	if (!key[keyNum] && oldkey[keyNum])
	{
		return true;
	}
	return false;
}

bool MCB::Input::IsMouseDown(size_t MouseBotton)
{
	if (mouse.rgbButtons[MouseBotton]) return true;
	return false;
}

bool MCB::Input::IsMouseNDown(size_t MouseBotton)
{
	if (!mouse.rgbButtons[MouseBotton]) return true;
	return false;
}

bool MCB::Input::IsMousePress(size_t MouseBotton)
{
	if (mouse.rgbButtons[MouseBotton] && oldmouse.rgbButtons[MouseBotton]) return true;
	return false;
}

bool MCB::Input::IsMouseTrigger(size_t MouseBotton)
{
	if (mouse.rgbButtons[MouseBotton] && !oldmouse.rgbButtons[MouseBotton]) return true;
	return false;
}

bool MCB::Input::IsMouseRelease(size_t MouseBotton)
{
	if (!mouse.rgbButtons[MouseBotton] && oldmouse.rgbButtons[MouseBotton])
	{
		return true;
	}
	return false;
}

MCB::Mouse MCB::Input::GetMousePosition()
{
	MCB::Mouse m;
	m.x = mouse.lX;
	m.y = mouse.lY;
	m.z = mouse.lZ;
	return m;
}

