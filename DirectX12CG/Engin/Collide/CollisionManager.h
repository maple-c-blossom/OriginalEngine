#pragma once
#include <forward_list>
#include "RayCastHit.h"
#include "MeshCollider.h"
#include "RayCollider.h"
#include <memory>
namespace MCB
{
	class BaseCollider;
	class CollisionManager final
	{
	public:
		static CollisionManager* GetInstance();
		inline void AddCollider(std::unique_ptr<BaseCollider> coll) { colliders_.push_front(move(coll)); }
		inline void RemoveCollider(BaseCollider* coll) { colliders_.remove_if(
			[&](auto& itr) {return itr.get() == coll; }); }
		void CheckAllCollision();
		bool Raycast( Ray& ray, RayCastHit* hitinfo = nullptr,  float maxDistance = D3D12_FLOAT32_MAX);
		bool Raycast( Ray& ray,  uint16_t attribute, RayCastHit* hitinfo = nullptr,  float maxDistance = D3D12_FLOAT32_MAX);
		
	private:
		CollisionManager() = default;
		CollisionManager(const CollisionManager&) = delete;
		~CollisionManager() = default;
		CollisionManager& operator= (const CollisionManager&) = delete;

		std::forward_list<std::unique_ptr<BaseCollider>>colliders_;
	};
}
