#pragma once
#include "BaseCollider.h"
#include "Vector3D.h"
#include "Pading.h"
namespace MCB
{
	class RayCollider:public BaseCollider,public Ray
	{
    public:
		RayCollider(const Vector3D& offset = { 0,0,0 },const Vector3D& rayVec = { 0,0,1 },
			 float radius = 0.f,float range = 10000.f);
        void Update() override;
		 void SetRadius(float radius);
		 void SetRange(float range);
    private:
		Vector3D offset_;
		Vector3D rayVec_;
        float radius_;
		float range_;
		Byte4 pad;
	};

}

