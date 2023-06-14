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
			view_.eye_ = { eyePos.x_,eyePos.y_,eyePos.z_ };
		}

		void SetViewTarget(const Float3& targetPos)
		{
			view_.target_ = { targetPos.x_,targetPos.y_,targetPos.z_ };
		}

		void SetViewUp(const Float3& up)
		{
			view_.up_ = { up.x_, up.y_, up.z_ };
		}

		void SetProjectionFarZ(float farZ)
		{
			projection_.farZ_ = farZ;
		}

		void SetProjectionFovAngle( float fovAngle)
		{
			projection_.fovAngle_ = fovAngle;
		}

		void SetProjection( float nearZ)
		{
			projection_.nearZ_ = nearZ;
		}

		void MatrixUpdate();

	};

}

