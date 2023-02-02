#pragma once
#include "Collider.h"
#include "Object3d.h"
#include "CollisionInfomation.h"

namespace MCB
{
	class BaseCollider
	{
	public:
		friend class CollisionManager;
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
			object3d->OffCollision(&info);
		}

		inline void SetAttribute(unsigned short attribute)
		{
			this->attribute = attribute;
		}
		inline void AddAttribute(unsigned short attribute)
		{
			this->attribute |= attribute;
		}
		inline void RemoveAttribute(unsigned short attribute)
		{
			this->attribute &= !attribute;
		}
		inline Object3d* GetObject3D() { return object3d; }
	protected:
		unsigned short attribute = 0b1111111111111111;
		Object3d* object3d = nullptr;
		MCB::PrimitiveType primitive = MCB::PrimitiveType::SPHERE;

	};
}
