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
		static void DeleteInstace();
		static GamePad* GetInitInstance();

		Float3 LStick{ 0,0,0 };
		Float3 RStick{ 0,0,0 };

		Float2 RTrriger = { 0,0 };
		Float2 LTrriger = { 0,0 };
		XINPUT_STATE state[4] = {};
		XINPUT_STATE oldstate[4] = {};
		void GetState();
		bool IsButtonDown(int Button, short int Userindex = 0);
		bool IsButtonNDown(int Button, short int Userindex = 0);
		bool IsButtonPress(int Button, short int Userindex = 0);
		bool IsButtonTrigger(int Button, short int Userindex = 0);
		bool IsButtonRelease(int Button, short int Userindex = 0);
		Float3 IsInputLStick(short int Userindex = 0);
		Float3 IsInputRStick(short int Userindex = 0);
		Float2 IsInputLTrriger(short int Userindex = 0);
		Float2 IsInputRTrriger(short int Userindex = 0);
		void Vibration(int LeftMotorSpeed, int RightMotorSpeed, short int Userindex = 0);
		void StopVibration(short int Userindex = 0);
	};

}

