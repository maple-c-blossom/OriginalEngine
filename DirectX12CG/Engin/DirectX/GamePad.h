#pragma once
#include <Windows.h>
#include <Xinput.h>
#include "MCBFloat.h"
#pragma comment (lib, "xinput.lib")

#define	GAMEPAD_UP		XINPUT_GAMEPAD_DPAD_UP
#define	GAMEPAD_DOWN	XINPUT_GAMEPAD_DPAD_DOWN
#define	GAMEPAD_LEFT	XINPUT_GAMEPAD_DPAD_LEFT
#define	GAMEPAD_RIGHT	XINPUT_GAMEPAD_DPAD_RIGHT
#define	GAMEPAD_START	XINPUT_GAMEPAD_START
#define	GAMEPAD_BACK	XINPUT_GAMEPAD_BACK
#define	GAMEPAD_STICK_L	XINPUT_GAMEPAD_LEFT_THUMB
#define	GAMEPAD_STICK_R	XINPUT_GAMEPAD_RIGHT_THUMB
#define	GAMEPAD_LB		XINPUT_GAMEPAD_LEFT_SHOULDER
#define	GAMEPAD_RB		XINPUT_GAMEPAD_RIGHT_SHOULDER
#define	GAMEPAD_A		XINPUT_GAMEPAD_A
#define	GAMEPAD_B		XINPUT_GAMEPAD_B
#define	GAMEPAD_X		XINPUT_GAMEPAD_X
#define	GAMEPAD_Y		XINPUT_GAMEPAD_Y

namespace MCB
{
	class GamePad
	{
	private:
		GamePad() {};
		GamePad(const GamePad& input) {};
		~GamePad() {};
		GamePad& operator= (const GamePad& Input) {};
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

