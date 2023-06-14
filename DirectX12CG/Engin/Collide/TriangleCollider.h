#pragma once
#include "BaseCollider.h"
#include "Vector3D.h"

namespace MCB
{
    class TriangleCollider : public BaseCollider,public Triangle
    {
    public:
        TriangleCollider(const Vector3D& offset = { 0,0,0 },const Vector3D& normal = {0,0,1}, 
           const std::array<Vector3D, 3>& vertexPoint = {Vector3D(-1.f,0.f,0.f), Vector3D(1.f,0.f,0.f), Vector3D(0.f,1.f,0.f)}) 
        { 
            offset_ = offset, normal_ = normal,vertexPoint_ = vertexPoint; 
            primitive_ = PrimitiveType::TRIANGLE; 
        };
        void Update() override;
        inline void SetVertexPoint(const Vector3D& point, int32_t index) { vertexPoint_[index] = point; }
    private:
        std::array<Vector3D, 3> vertexPoint_;
        Vector3D offset_;
        Vector3D normal_;

    };

}

