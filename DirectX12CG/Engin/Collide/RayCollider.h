#pragma once
#include "BaseCollider.h"
#include "Vector3D.h"
namespace MCB
{
	class RayCollider:public BaseCollider,public Ray
	{
    public:
        RayCollider(const Vector3D& offset = { 0,0,0 },const Vector3D& rayVec = { 0,0,1 },
             float radius = 0.f, float range = 10000.f) 
        {
            offset_ = offset; radius_ = radius; rayVec_ = rayVec; 
            range_ = range; primitive_ = PrimitiveType::RAY; 
        };
        void Update() override;
        inline void SetRadius( float radius) { radius_ = radius; }
        inline void SetRange( float range) { range_ = range; }
    private:
		Vector3D offset_;
		Vector3D rayVec_;
        float radius_;
		float range_;
	};

}

