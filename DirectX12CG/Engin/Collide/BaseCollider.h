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
		inline void SetObject(Object3d* object) { object3d_ = object; };
		virtual void Update() = 0;
		inline MCB::PrimitiveType GetPrimitive() { return primitive_; }
		inline void OnCollision(const CollisionInfomation& info)
		{
			object3d_->OnCollision(info);
		}
		inline void OffCollision(const CollisionInfomation& info)
		{
			object3d_->OffCollision(&info);
		}

		inline void SetAttribute( uint16_t attribute)
		{
			attribute_ = attribute;
		}
		inline void AddAttribute( uint16_t attribute)
		{
			attribute_ |= attribute;
		}
		inline void RemoveAttribute( uint16_t attribute)
		{
			attribute_ &= !attribute;
		}
		inline Object3d* GetObject3D() { return object3d_; }
	protected:
		uint16_t attribute_ = ATTRIBUTE_LANDSHAPE;
		Object3d* object3d_ = nullptr;
		MCB::PrimitiveType primitive_ = MCB::PrimitiveType::SPHERE;

	};
}
