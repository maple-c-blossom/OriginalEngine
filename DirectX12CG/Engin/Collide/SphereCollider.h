#pragma once
#include "BaseCollider.h"
#include "Collider.h"
#include "Vector3D.h"

namespace MCB
{
    class SphereCollider :public BaseCollider, public Sphere
    {
    public:
		SphereCollider(const Vector3D& offset = { 0,0,0 },float radius = 1.0f);
        void Update() override;
		 void SetRadius(float radius);
		 float GetRaius();
    private:
        Vector3D offset_;
        float radius_;

    };
}
