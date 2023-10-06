#pragma once
#include "BaseCollider.h"
#include "Vector3D.h"
namespace MCB
{
    class BoxCollider :public BaseCollider,public Box
    {
    public:
        BoxCollider(const Vector3D& offset = { 0,0,0 }, Float3 size = {1.f,1.f,1.f}) ;
        void Update() override;
		 void SetSize(const Float3& size);
    private:
        Vector3D offset_;
        Float3 size_;

    };
}

