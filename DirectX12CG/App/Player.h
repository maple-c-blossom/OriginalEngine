#pragma once
#include "Object3d.h"
#include "SphereCollider.h"
namespace MCB
{
    class Player :public Object3d
    {
    private:
        bool isGraund = true;
        Vector3D fallV;
    public:
        void Init();
        void Update();
        void UpdateMatrix();
        void Draw();
    };

}

