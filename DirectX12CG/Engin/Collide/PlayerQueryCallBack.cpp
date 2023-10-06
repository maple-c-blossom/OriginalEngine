#include "PlayerQueryCallBack.h"

MCB::PlayerQueryCallBack::PlayerQueryCallBack(Sphere* sphere):sphere_(sphere) {}

bool MCB::PlayerQueryCallBack::OnQueryHit(const QueryHit& info)
{
	const Vector3D up = { 0,1,0 };
	Vector3D reject = info.reject;
	reject.V3Norm();
	float cos = reject.GetV3Dot(up);
	const float threshold = cosf(ConvertRadius(30.0f));
	if (-threshold < cos && cos < threshold)
	{
		sphere_->centerPosition_ += info.reject;
		move += info.reject;
		if (info.obj->nameId_ == "block")
		{
			block = true;
		}
	}
	return true;

}
