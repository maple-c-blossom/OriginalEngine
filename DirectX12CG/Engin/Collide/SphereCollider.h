#pragma once
#include "BaseCollider.h"
#include "Collider.h"
#include "Vector3D.h"

namespace MCB
{
    class SphereCollider :public BaseCollider, public Sphere
    {
    public:
        SphereCollider(const Vector3D& offset = { 0,0,0 }, float radius = 1.0f) 
        {
            offset_ = offset, radius_ = radius; primitive_ = PrimitiveType::SPHERE; 
        };
        void Update() override;
        inline void SetRadius( float radius) { radius_ = radius; }
        inline float GetRaius() { return radius_; }
    private:
        Vector3D offset_;
        float radius_;

    };
}
