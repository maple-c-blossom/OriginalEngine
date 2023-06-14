#include "GamePad.h"
#define INPUT_DEADZONE 6000
#define INPUT_TRRIGERDEADZONE 10
#define MAX_MAGNITUDE 32767
#define MAX_TRRIGERMAGNITUDE 255
using namespace MCB;

void MCB::GamePad::GetState()
{
	DWORD result;
	for (int32_t i = 0; i < 4; i++)
	{
		oldstate_[i] = state_[i];
		result = XInputGetState(i, &state_[i]);
		if (result != ERROR_SUCCESS) break;
	}
}

bool MCB::GamePad::IsButtonDown( int32_t Button, int16_t Userindex)
{
	if (state_[Userindex].Gamepad.wButtons & Button)
	{
		return true;
	}
	return false;
}

bool MCB::GamePad::IsButtonNDown( int32_t Button, int16_t Userindex)
{
	if (!(state_[Userindex].Gamepad.wButtons & Button))
	{
		return true;
	}
	return false;
}

bool MCB::GamePad::IsButtonPress( int32_t Button,  int16_t Userindex)
{
	if ((state_[Userindex].Gamepad.wButtons & Button) && (oldstate_[Userindex].Gamepad.wButtons & Button))
	{
		return true;
	}
	return false;
}

bool MCB::GamePad::IsButtonTrigger( int32_t Button,  int16_t Userindex)
{
	if ((state_[Userindex].Gamepad.wButtons & Button) && !(oldstate_[Userindex].Gamepad.wButtons & Button))
	{
		return true;
	}
	return false;
}

bool MCB::GamePad::IsButtonRelease( int32_t Button,  int16_t Userindex)
{
	if (!(state_[Userindex].Gamepad.wButtons & Button) && (oldstate_[Userindex].Gamepad.wButtons & Button))
	{
		return true;
	}
	return false;
}

Float3 MCB::GamePad::IsInputLStick( int16_t Userindex)
{
	float LX = state_[Userindex].Gamepad.sThumbLX;
	float LY = state_[Userindex].Gamepad.sThumbLY;

	//determine how far the controller is pushed
	float magnitude = (float)sqrt(LX * LX + LY * LY);

	//determine the direction the controller is pushed
	float normalizedLX = LX / magnitude;
	float normalizedLY = LY / magnitude;

	float normalizedMagnitude = 0;

	//check if the controller is outside a circular dead zone
	if (magnitude > INPUT_DEADZONE)
	{
		//clip the magnitude at its expected maximum value
		if (magnitude > MAX_MAGNITUDE) magnitude = MAX_MAGNITUDE;

		//adjust magnitude relative to the end of the dead zone
		magnitude -= INPUT_DEADZONE;

		//optionally normalize the magnitude with respect to its expected range
		//giving a magnitude value of 0.0 to 1.0
		normalizedMagnitude = magnitude / (MAX_MAGNITUDE - INPUT_DEADZONE);
		return { normalizedLX,normalizedLY,normalizedMagnitude };
	}
	else //if the controller is in the deadzone zero out the magnitude
	{
		magnitude = 0.0;
		normalizedMagnitude = 0.0;
		return { 0,0,0 };
	}

	//repeat for right thumb stick
}

Float3 MCB::GamePad::IsInputRStick( int16_t Userindex)
{
	float RX = state_[Userindex].Gamepad.sThumbRX;
	float RY = state_[Userindex].Gamepad.sThumbRY;

	//determine how far the controller is pushed
	float magnitude = (float)sqrt(RX * RX + RY * RY);

	//determine the direction the controller is pushed
	float normalizedRX = RX / magnitude;
	float normalizedRY = RY / magnitude;

	float normalizedMagnitude = 0;

	//check if the controller is outside a circular dead zone
	if (magnitude > INPUT_DEADZONE)
	{
		//clip the magnitude at its expected maximum value
		if (magnitude > MAX_MAGNITUDE) magnitude = MAX_MAGNITUDE;

		//adjust magnitude relative to the end of the dead zone
		magnitude -= INPUT_DEADZONE;

		//optionally normalize the magnitude with respect to its expected range
		//giving a magnitude value of 0.0 to 1.0
		normalizedMagnitude = magnitude / (MAX_MAGNITUDE - INPUT_DEADZONE);
		return { normalizedRX,normalizedRY,normalizedMagnitude };
	}
	else //if the controller is in the deadzone zero out the magnitude
	{
		magnitude = 0.0;
		normalizedMagnitude = 0.0;
		return { 0,0,0 };
	}

	//repeat for right thumb stick
}

Float2 MCB::GamePad::IsInputLTrriger( int16_t Userindex)
{
	float LT = state_[Userindex].Gamepad.bLeftTrigger;
	//determine how far the controller is pushed
	float magnitude =(float)sqrt(LT* LT);

	//determine the direction the controller is pushed
	float normalizedLT = LT / magnitude;

	float normalizedMagnitude = 0;

	//check if the controller is outside a circular dead zone
	if (magnitude > INPUT_TRRIGERDEADZONE)
	{
		//clip the magnitude at its expected maximum value
		if (magnitude > MAX_TRRIGERMAGNITUDE) magnitude = MAX_TRRIGERMAGNITUDE;

		//adjust magnitude relative to the end of the dead zone
		magnitude -= INPUT_TRRIGERDEADZONE;

		//optionally normalize the magnitude with respect to its expected range
		//giving a magnitude value of 0.0 to 1.0
		normalizedMagnitude = magnitude / (MAX_TRRIGERMAGNITUDE - INPUT_TRRIGERDEADZONE);
		return { normalizedLT,normalizedMagnitude };
	}
	else //if the controller is in the deadzone zero out the magnitude
	{
		magnitude = 0.0;
		normalizedMagnitude = 0.0;
		return { 0,0};
	}
}

Float2 MCB::GamePad::IsInputRTrriger( int16_t Userindex)
{
	float RT = state_[Userindex].Gamepad.bRightTrigger;
	//determine how far the controller is pushed
	float magnitude = (float)sqrt(RT * RT);

	//determine the direction the controller is pushed
	float normalizedRT = RT / magnitude;

	float normalizedMagnitude = 0;

	//check if the controller is outside a circular dead zone
	if (magnitude > INPUT_TRRIGERDEADZONE)
	{
		//clip the magnitude at its expected maximum value
		if (magnitude > MAX_TRRIGERMAGNITUDE) magnitude = MAX_TRRIGERMAGNITUDE;

		//adjust magnitude relative to the end of the dead zone
		magnitude -= INPUT_TRRIGERDEADZONE;

		//optionally normalize the magnitude with respect to its expected range
		//giving a magnitude value of 0.0 to 1.0
		normalizedMagnitude = magnitude / (MAX_TRRIGERMAGNITUDE - INPUT_TRRIGERDEADZONE);
		return { normalizedRT,normalizedMagnitude };
	}
	else //if the controller is in the deadzone zero out the magnitude
	{
		magnitude = 0.0;
		normalizedMagnitude = 0.0;
		return { 0,0 };
	}
}

void MCB::GamePad::Vibration( int32_t LeftMotorSpeed,  int32_t RightMotorSpeed,  int16_t Userindex)
{
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = LeftMotorSpeed; // use any value between 0-65535 here
	vibration.wRightMotorSpeed = RightMotorSpeed; // use any value between 0-65535 here
	XInputSetState(Userindex, &vibration);
}

void MCB::GamePad::StopVibration( int16_t Userindex)
{
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = 0; // use any value between 0-65535 here
	vibration.wRightMotorSpeed = 0; // use any value between 0-65535 here
	XInputSetState(Userindex, &vibration);
}

GamePad* MCB::GamePad::GetInstance()
{
	static GamePad instance;
	return &instance;
}

void MCB::GamePad::AllStopVibration()
{
	for (int32_t i = 0; i < 4; i++)
	{
		GamePad::GetInstance()->StopVibration(i);
	}

	//delete GamePad::GetInstance();
}

GamePad* MCB::GamePad::GetInitInstance()
{
	static GamePad* instance = GamePad::GetInstance();
	instance->GetState();
	return instance;
}