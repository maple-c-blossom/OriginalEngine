#pragma once
#include "IgnoreWarning.h"
WarningIgnoreBegin
#include <forward_list>
#include <memory>
WarningIgnoreEnd
#include "RayCastHit.h"
#include "MeshCollider.h"
#include "RayCollider.h"
#include "Query.h"

namespace MCB 
{
	class BaseCollider;
	class CollisionManager final
	{
	public:
		static CollisionManager* GetInstance();
		 BaseCollider* AddCollider(std::unique_ptr<BaseCollider> coll);
		void RemoveCollider(BaseCollider* coll);
		void CheckAllCollision();
		bool Raycast( Ray& ray, RayCastHit* hitinfo = nullptr,  float maxDistance = D3D12_FLOAT32_MAX);
		bool Raycast( Ray& ray,  uint16_t attribute, RayCastHit* hitinfo = nullptr,  float maxDistance = D3D12_FLOAT32_MAX);
		void QuerySphere(const Sphere& sphere, QueryCallBack* callBack, uint16_t attribute = static_cast<uint16_t>(0xffffffff));
	private:
		CollisionManager() = default;
		CollisionManager(const CollisionManager&) = delete;
		~CollisionManager() = default;
		CollisionManager& operator= (const CollisionManager&) = delete;

		std::forward_list<std::unique_ptr<BaseCollider>>colliders_;
	};
}
