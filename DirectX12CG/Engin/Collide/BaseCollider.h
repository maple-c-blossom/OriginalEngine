#pragma once
#include "Collider.h"
#include "Object3d.h"
#include "CollisionInfomation.h"

namespace MCB
{
	class BaseCollider
	{
	public:
		BaseCollider() = default;
		virtual ~BaseCollider() = default;
		inline void SetObject(Object3d* object) { this->object3d = object; };
		virtual void Update() = 0;
		inline MCB::PrimitiveType GetPrimitive() { return primitive; }
		inline void OnCollision(const CollisionInfomation& info)
		{
			object3d->OnCollision(info);
		}
		inline void OffCollision(const CollisionInfomation& info)
		{
			object3d->OffCollision(info);
		}
		inline Object3d* GetObject3D() { return object3d; }
	protected:
		Object3d* object3d = nullptr;
		MCB::PrimitiveType primitive = MCB::PrimitiveType::SPHERE;

	};
}
