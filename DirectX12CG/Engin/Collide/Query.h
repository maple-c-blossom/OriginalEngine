#pragma once
#include "Vector3d.h"
namespace MCB
{
	class Object3d;
	class BaseCollider;

	struct QueryHit
	{
		Object3d* obj = nullptr;
		BaseCollider* baseColl = nullptr;
		Vector3D inter;
		Vector3D reject;
	};
	class QueryCallBack
	{
	public:
		QueryCallBack() = default;
		virtual ~QueryCallBack() = default;

		virtual bool OnQueryHit(const QueryHit& info) = 0;
	};
}
