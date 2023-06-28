#pragma once
#include "Query.h"
#include "BaseCollider.h"
#include "Collider.h"
#include "Vector3D.h"
#include "Util.h"
namespace MCB
{
	class PlayerQueryCallBack :public MCB::QueryCallBack
	{
	public:
		PlayerQueryCallBack(Sphere* sphere):sphere_(sphere) {};
		Sphere* sphere_ = nullptr;
		Vector3D move = {};
		bool OnQueryHit(const QueryHit& info)override;

	};
}

