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
		Input* input = Input::GetInstance();

		float disEyeTarget;
		Vector2D rotAngle;
		Vector2D cursorSpd;
		Vector3D eye;
		Vector3D target;
		//Vector3D up;
	public:
		void Inilialize()override;
		void Update()override;
	};
}

