#include "GrabCallBack.h"


bool MCB::GrabCallBack::OnQueryHit(const QueryHit& info)
{
	const Vector3D up = { 0,1,0 };
	Vector3D reject = info.reject;
	reject.V3Norm();
	float cos = reject.GetV3Dot(up);
	const float threshold = cosf(ConvertRadius(30.0f));
	hited = false;
	if ( -threshold < cos && cos < threshold )
	{
		if ( info.obj->nameId_ == "grabObj" )
		{
			hited = true;
			return true;
		}

	}
	return false;

}
