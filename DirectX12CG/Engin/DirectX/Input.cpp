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

	dx12->result_ = DirectInput8Create(dxWindow->window_.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&dinput_, nullptr);
	assert(SUCCEEDED(dx12->result_));
	//�L�[�{�[�h�f�o�C�X�̐���-----------------
#pragma region �L�[�{�[�h�f�o�C�X�̐���
	dx12->result_ = dinput_->CreateDevice(GUID_SysKeyboard, &devkeyboard_, NULL);
	assert(SUCCEEDED(dx12->result_));
#pragma endregion �L�[�{�[�h�f�o�C�X�̐���

	//���̓f�[�^�`���Z�b�g--------------------------------
	dx12->result_ = devkeyboard_->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(dx12->result_));
	//---------------------------------

	//�r�����x������-------------------------------------------------------------
	dx12->result_ = devkeyboard_->SetCooperativeLevel(dxWindow->hwnd_,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(dx12->result_));
	//--------------------------

	//�}�E�X�f�o�C�X�̐���-----------------
#pragma region �}�E�X�f�o�C�X�̐���
	dx12->result_ = dinput_->CreateDevice(GUID_SysMouse, &devmouse_, NULL);
	assert(SUCCEEDED(dx12->result_));
#pragma endregion �}�E�X�f�o�C�X�̐���
	//--------------------------
	// ���̓f�[�^�`���̃Z�b�g
	dx12->result_ = devmouse_->SetDataFormat(&c_dfDIMouse2); // �W���`��
	assert(SUCCEEDED(dx12->result_));
	// �r�����䃌�x���̃Z�b�g
	dx12->result_ = devmouse_->SetCooperativeLevel(
		dxWindow->hwnd_, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(dx12->result_));

#pragma endregion ���͌n������
	//----------------
}

void Input::UpDateInit()
{
	
	//�L�[�{�[�h������-------------------------------------
#pragma region �L�[�{�[�h������

	//�L�[�{�[�h���̎擾�J�n-----------------
	devkeyboard_->Acquire();
	//----------------------------
	//�S�L�[�̓��͏�Ԃ��擾����---------------------------
	KeyUpdate();
	Dx12::GetInstance()->result_ = devkeyboard_->GetDeviceState(sizeof(key_), key_);
	//----------------------------

	// �}�E�X
	devmouse_->Acquire();	// �}�E�X����J�n
	MouseUpdate();
	// �}�E�X�̓���
	Dx12::GetInstance()->result_ = devmouse_->GetDeviceState(sizeof(mouse_), &mouse_);
#pragma endregion �L�[�{�[�h������
		//----------------------------------------
	gamePad_->GetState();
	gamePad_->LStick_ = gamePad_->IsInputLStick();
	gamePad_->RStick_ = gamePad_->IsInputRStick();
	gamePad_->LTrriger_ = gamePad_->IsInputLTrriger();
	gamePad_->RTrriger_ = gamePad_->IsInputRTrriger();
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

