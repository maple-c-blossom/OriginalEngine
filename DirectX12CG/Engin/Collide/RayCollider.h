#pragma once
#include "BaseCollider.h"
#include "Vector3D.h"
namespace MCB
{
	class RayCollider:public BaseCollider,public Ray
	{
    public:
        RayCollider(Vector3D offset = { 0,0,0 }, Vector3D rayVec = { 0,0,1 }, float radius = 0.f, float range = 10000.f) { this->offset = offset; this->radius = radius; this->rayVec = rayVec; this->range = range; primitive = PrimitiveType::RAY; };
        void Update() override;
        inline void SetRadius(float radius) { this->radius = radius; }
        inline void SetRange(float range) { this->range = range; }
    private:
		Vector3D offset;
		Vector3D rayVec;
        float radius;
		float range;
	};

}

