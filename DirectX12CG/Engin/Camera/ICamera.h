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
		virtual ~ICamera() {};
		virtual void Inilialize() = 0;
		virtual void Update() = 0;
		View* GetView()
		{
			return &view_;
		}
		Projection* GetProjection()
		{
			return &projection_;
		}

		void SetViewEye(const Float3& eyePos)
		{
			view_.eye_ = { eyePos.x,eyePos.y,eyePos.z };
		}

		void SetViewTarget(const Float3& targetPos)
		{
			view_.target_ = { targetPos.x,targetPos.y,targetPos.z };
		}

		void SetViewUp(const Float3& up)
		{
			view_.up_ = { up.x, up.y, up.z };
		}

		void SetProjectionFarZ(const float& farZ)
		{
			projection_.farZ_ = farZ;
		}

		void SetProjectionFovAngle(const float& fovAngle)
		{
			projection_.fovAngle_ = fovAngle;
		}

		void SetProjection(const float& nearZ)
		{
			projection_.nearZ_ = nearZ;
		}

		void MatrixUpdate();

	};

}

