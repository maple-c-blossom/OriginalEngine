#pragma once
#include "Object3d.h"
#include "SphereCollider.h"
#include "Input.h"
namespace MCB
{
    class Player :public Object3d
    {
    private:
        bool isGraund_ = true;
        Vector3D fallV_;
        Input* input_ = Input::GetInstance();
        float speedFront_ = 0.0f;
        float speedRight_ = 0.0f;
        float rotasionSpeed_ = 0.015f;
        float speed_ = 0.125f;
        float srowspeed_ = 0.075f;
        float maxspeed_ = 0.5f;
        MCB::Vector3D rightVec_ = {1,0,0};
    public:
        void Init();
        void Update();
        void Draw();
        void Move();
    };

}

