#pragma once
#include "ICamera.h"
#include "Input.h"
#include "Vecter2D.h"
#include "Vector3D.h"

namespace MCB
{
	class DebugCamera :public ICamera
	{
	private:
		Input* input_ = Input::GetInstance();

		float disEyeTarget_;
		Vector2D rotAngle_;
		Vector2D cursorSpd_;
		Vector3D eye_;
		Vector3D target_;
		//Vector3D up;
	public:
		void Inilialize()override;
		void Update()override;
	};
}

