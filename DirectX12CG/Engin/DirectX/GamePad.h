#pragma once
#include "IgnoreWarning.h"
WarningIgnoreBegin
#include <Windows.h>
#include <Xinput.h>
WarningIgnoreEnd
#include "MCBFloat.h"
#pragma comment (lib, "xinput.lib")

enum GAMEPAD
{
	GAMEPAD_UP		= XINPUT_GAMEPAD_DPAD_UP,
	GAMEPAD_DOWN	= XINPUT_GAMEPAD_DPAD_DOWN,
	GAMEPAD_LEFT	= XINPUT_GAMEPAD_DPAD_LEFT,
	GAMEPAD_RIGHT	= XINPUT_GAMEPAD_DPAD_RIGHT,
	GAMEPAD_START	= XINPUT_GAMEPAD_START,
	GAMEPAD_BACK	= XINPUT_GAMEPAD_BACK,
	GAMEPAD_STICK_L	= XINPUT_GAMEPAD_LEFT_THUMB,
	GAMEPAD_STICK_R	= XINPUT_GAMEPAD_RIGHT_THUMB,
	GAMEPAD_LB		= XINPUT_GAMEPAD_LEFT_SHOULDER,
	GAMEPAD_RB		= XINPUT_GAMEPAD_RIGHT_SHOULDER,
	GAMEPAD_A		= XINPUT_GAMEPAD_A,
	GAMEPAD_B		= XINPUT_GAMEPAD_B,
	GAMEPAD_X		= XINPUT_GAMEPAD_X,
	GAMEPAD_Y		= XINPUT_GAMEPAD_Y,

};

static const int32_t INPUT_DEADZONE = 6000;
static const int32_t INPUT_TRRIGERDEADZONE = 10;
static const int32_t MAX_MAGNITUDE = 32767;
static const int32_t MAX_TRRIGERMAGNITUDE = 255;
namespace MCB
{
	class GamePad
	{
	private:
		GamePad();
		GamePad(const GamePad&);
		~GamePad();
		GamePad& operator= (const GamePad&);
	public:
		static GamePad* GetInstance();
		static void AllStopVibration();
		static GamePad* GetInitInstance();

		Float3 LStick_{ 0,0,0 };
		Float3 RStick_{ 0,0,0 };

		Float2 RTrriger_ = { 0,0 };
		Float2 LTrriger_ = { 0,0 };
		XINPUT_STATE state_[4] = {};
		XINPUT_STATE oldstate_[4] = {};
		void GetState();
		bool IsButtonDown( int32_t Button,  int16_t Userindex = 0);
		bool IsButtonNDown( int32_t Button, int16_t Userindex = 0);
		bool IsButtonPress( int32_t Button, int16_t Userindex = 0);
		bool IsButtonTrigger( int32_t Button, int16_t Userindex = 0);
		bool IsButtonRelease( int32_t Button, int16_t Userindex = 0);
		Float3 IsInputLStick( int16_t Userindex = 0);
		Float3 IsInputRStick( int16_t Userindex = 0);
		Float2 IsInputLTrriger( int16_t Userindex = 0);
		Float2 IsInputRTrriger( int16_t Userindex = 0);
		void Vibration( int32_t LeftMotorSpeed,  int32_t RightMotorSpeed,  int16_t Userindex = 0);
		void StopVibration( int16_t Userindex = 0);
	};

}

