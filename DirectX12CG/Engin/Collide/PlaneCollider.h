#pragma once
#include "BaseCollider.h"
#include "Vector3D.h"

namespace MCB
{
    class PlaneCollider :public BaseCollider,public Plane
    {
    public:
        PlaneCollider(Vector3D normal = { 0,0,0 }, float originDistance = 1.0f) { this->normal = normal, this->originDistance = originDistance; primitive = PrimitiveType::PLANE; };
        void Update() override;
        inline void SetDistance(float originDistance) { this->originDistance = originDistance; }
    private:
        Vector3D normal = { 0,1,0 };
        float originDistance = 0;//Œ´“_‚©‚ç‚Ì‹——£

    };
}
