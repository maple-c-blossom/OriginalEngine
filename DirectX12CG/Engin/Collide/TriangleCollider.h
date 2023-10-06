#pragma once
#include "BaseCollider.h"
#include "Vector3D.h"

namespace MCB
{
    class TriangleCollider : public BaseCollider,public Triangle
    {
    public:
		TriangleCollider(const Vector3D& offset = { 0,0,0 },const Vector3D& normal = { 0,0,1 },
		   const std::array<Vector3D,3>& vertexPoint = { Vector3D(-1.f,0.f,0.f), Vector3D(1.f,0.f,0.f), Vector3D(0.f,1.f,0.f) });
        void Update() override;
		void SetVertexPoint(const Vector3D& point,size_t index);
    private:
        std::array<Vector3D, 3> vertexPoint_;
        Vector3D offset_;
        Vector3D normal_;
		Byte4 pad1;
    };

}

