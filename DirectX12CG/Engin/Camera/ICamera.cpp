#include "ICamera.h"
using namespace MCB;
View* MCB::ICamera::GetView()
{
	return &view_;
}

Projection* MCB::ICamera::GetProjection()
{
	return &projection_;
}

void MCB::ICamera::SetViewEye(const Float3& eyePos)
{
	view_.eye_ = { eyePos.x_,eyePos.y_,eyePos.z_ };
}

void MCB::ICamera::SetViewTarget(const Float3& targetPos)
{
	view_.target_ = { targetPos.x_,targetPos.y_,targetPos.z_ };
}

void MCB::ICamera::SetViewUp(const Float3& up)
{
	view_.up_ = { up.x_, up.y_, up.z_ };
}

void MCB::ICamera::SetProjectionFarZ(float farZ)
{
	projection_.farZ_ = farZ;
}

void MCB::ICamera::SetProjectionFovAngle(float fovAngle)
{
	projection_.fovAngle_ = fovAngle;
}
void MCB::ICamera::SetProjection(float nearZ)
{
	projection_.nearZ_ = nearZ;
}
void MCB::ICamera::MatrixUpdate()
{
	projection_.UpdataMatrixProjection();
	view_.UpDateMatrixView();
}
