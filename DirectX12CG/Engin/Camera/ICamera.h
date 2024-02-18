#pragma once
#include "View.h"
#include "Projection.h"
#include "MCBFloat.h"
#include "Util.h"


namespace MCB
{
	class ICamera
	{
	protected:
		View view_;
		Projection projection_;
	public:
		Vector3D targetObjctVec_;
		virtual ~ICamera() {};
		virtual void Inilialize() = 0;
		virtual void Update() = 0;
		View* GetView();
		Projection* GetProjection();

		void SetViewEye(const Float3& eyePos);

		void SetViewTarget(const Float3& targetPos);

		void SetViewUp(const Float3& up);


		void SetProjectionFarZ(float farZ);


		void SetProjectionFovAngle(float fovAngle);


		void SetProjection(float nearZ);


		void MatrixUpdate();

	};

}

