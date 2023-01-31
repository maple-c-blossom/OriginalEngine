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
		View view;
		Projection projection;
	public:

		virtual void Inilialize() = 0;
		virtual void Update() = 0;
		View* GetView()
		{
			return &view;
		}
		Projection* GetProjection()
		{
			return &projection;
		}

		void SetViewEye(Float3 eyePos)
		{
			view.eye = { eyePos.x,eyePos.y,eyePos.z };
		}

		void SetViewTarget(Float3 targetPos)
		{
			view.target = { targetPos.x,targetPos.y,targetPos.z };
		}

		void SetViewUp(Float3 up)
		{
			view.up = { up.x, up.y, up.z };
		}

		void SetProjectionFarZ(float farZ)
		{
			projection.farZ = farZ;
		}

		void SetProjectionFovAngle(float fovAngle)
		{
			projection.fovAngle = fovAngle;
		}

		void SetProjection(float nearZ)
		{
			projection.nearZ = nearZ;
		}

		void MatrixUpdate();

	};

}

