#pragma once
#include "BaseCollider.h"
#include "Vector3D.h"

namespace MCB
{
    class TriangleCollider : public BaseCollider,public Triangle
    {
    public:
        TriangleCollider(Vector3D offset = { 0,0,0 }, Vector3D normal = {0,0,1}, std::array<Vector3D, 3> vertexPoint = {Vector3D(-1.f,0.f,0.f), Vector3D(1.f,0.f,0.f), Vector3D(0.f,1.f,0.f)}) { this->offset = offset, this->normal = normal,this->vertexPoint = vertexPoint; primitive = PrimitiveType::TRIANGLE; };
        void Update() override;
        inline void SetVertexPoint(Vector3D point,int index) { vertexPoint[index] = point; }
    private:
        std::array<Vector3D, 3> vertexPoint;
        Vector3D offset;
        Vector3D normal;

    };

}

