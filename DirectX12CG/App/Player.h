#pragma once
#include "Object3d.h"
#include "SphereCollider.h"
namespace MCB
{
    class Player :public Object3d
    {
    private:
        bool isGraund_ = true;
        Vector3D fallV_;


    public:
        void Init();
        void Update();
        void Draw();
    };

}

