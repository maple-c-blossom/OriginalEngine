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
		inline void AddCollider(std::shared_ptr<BaseCollider> coll) { colliders.push_front(coll); }
		inline void RemoveCollider(std::shared_ptr<BaseCollider> coll) { colliders.remove(coll); }
		void CheckAllCollision();
		bool Raycast( Ray& ray, RayCastHit* hitinfo = nullptr, float maxDistance = D3D12_FLOAT32_MAX);
		bool Raycast( Ray& ray,unsigned short attribute, RayCastHit* hitinfo = nullptr, float maxDistance = D3D12_FLOAT32_MAX);
		
	private:
		CollisionManager() = default;
		CollisionManager(const CollisionManager&) = delete;
		~CollisionManager() = default;
		CollisionManager& operator= (const CollisionManager&) = delete;

		std::forward_list<std::shared_ptr<BaseCollider>>colliders;
	};
}
