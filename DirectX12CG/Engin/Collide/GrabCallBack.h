#pragma once
#include "Pading.h"
#include "Query.h"
#include "BaseCollider.h"
#include "Collider.h"
#include "Vector3D.h"
#include "Util.h"
namespace MCB
{
	class GrabCallBack :public MCB::QueryCallBack
	{
	public:
		bool hited = false;
		bool OnQueryHit(const QueryHit& info)override;

	};
}

