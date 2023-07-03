#pragma once
#include "BaseCollider.h"
#include "Vector3D.h"

namespace MCB
{
    class PlaneCollider :public BaseCollider,public Plane
    {
    public:
        PlaneCollider(const Vector3D& normal = { 0,0,0 },  float originDistance = 1.0f) 
        {
            normal_ = normal, originDistance_ = originDistance; primitive_ = PrimitiveType::PLANE; 
        };
        void Update() override;
        inline void SetDistance( float originDistance) { originDistance_ = originDistance; }
    private:
        Vector3D normal_ = { 0,1,0 };
        float originDistance_ = 0;//Œ´“_‚©‚ç‚Ì‹——£

    };
}
