#pragma once
#include "BaseCollider.h"
#include "Vector3D.h"

namespace MCB
{
    class PlaneCollider :public BaseCollider,public Plane
    {
    public:

		PlaneCollider(const Vector3D& normal = { 0,0,0 },float originDistance = 1.0f);
        void Update() override;
		void SetDistance(float originDistance);
    private:
        Vector3D normal_ = { 0,1,0 };
        float originDistance_ = 0;//原点からの距離

    };
}
