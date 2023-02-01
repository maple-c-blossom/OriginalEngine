#pragma once
#include "BaseCollider.h"
#include "Vector3D.h"
namespace MCB
{
    class BoxCollider :public BaseCollider,public Box
    {
    public:
        BoxCollider(Vector3D offset = { 0,0,0 }, Float3 size = {1.f,1.f,1.f}) { this->offset = offset, this->size = size; primitive = PrimitiveType::BOX; };
        void Update() override;
        inline void SetSize(Float3 size) { this->size = size; }
    private:
        Vector3D offset;
        Float3 size;

    };
}

