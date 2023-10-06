#pragma once
#include "Collider.h"
#include "Object3d.h"
#include "CollisionInfomation.h"
#include "Pading.h"
namespace MCB
{
	class BaseCollider
	{
	public:
		friend class CollisionManager;
		BaseCollider() = default;
		virtual ~BaseCollider() = default;
		void SetObject(Object3d* object);
		virtual void Update() = 0;
		MCB::PrimitiveType GetPrimitive();
		void OnCollision(const CollisionInfomation& info);
		void OffCollision(const CollisionInfomation& info);

		void SetAttribute(uint16_t attribute);
		void AddAttribute(uint16_t attribute);
		void RemoveAttribute(uint16_t attribute);
		Object3d* GetObject3D();
	protected:
		uint16_t attribute_ = ATTRIBUTE_LANDSHAPE;
		Byte6 pad1;
		Object3d* object3d_ = nullptr;
		MCB::PrimitiveType primitive_ = MCB::PrimitiveType::SPHERE;
		Byte4 pad2;

	};
}
