#pragma once
#include "Pading.h"
#include "Query.h"
#include "BaseCollider.h"
#include "Collider.h"
#include "Vector3D.h"
#include "Util.h"
namespace MCB
{
	class Object3d;
	class GrabQuery :public MCB::QueryCallBack
	{
	public:
		GrabQuery(Sphere* sphere);
		Sphere* sphere_ = nullptr;
		Object3d* obj_ = nullptr;
		Vector3D move = {};
		Vector3D inter = {};
		bool grab = false;
		Byte7 pad;
		bool OnQueryHit(const QueryHit& info)override;


	};
}

