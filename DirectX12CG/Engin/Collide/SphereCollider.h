#pragma once
#include "BaseCollider.h"
#include "Collider.h"
#include "Vector3D.h"

namespace MCB
{
    class SphereCollider :public BaseCollider, public Sphere
    {
    public:
        SphereCollider(Vector3D offset = { 0,0,0 }, float radius = 1.0f) { this->offset = offset, this->radius = radius; primitive = PrimitiveType::SPHERE; };
        void Update() override;
        inline void SetRadius(float radius) { this->radius = radius; }
        inline float GetRaius() { return radius; }
    private:
        Vector3D offset;
        float radius;

    };
}
