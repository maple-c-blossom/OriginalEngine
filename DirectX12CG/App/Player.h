#pragma once
#include "Object3d.h"
#include "SphereCollider.h"
#include "Input.h"
#include "Particle.h"
#include "vector"
#include "Timer.h"
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
        float defualtSpeed_ = 0.15f;
        float maxFrontSpeed_ = 0.7f;
        float maxspeed_ = 0.35f;
        float accelerator_ = 0.005f;
        float acceleratorfront_ = 0.05f;
        MCB::Vector3D rightVec_ = {1,0,0};
        Object3d* ground;
        Vector3D respownPosition_;
        float outYPosition = -20;
        float distoffSet = 0.00025f;
        Timer backTimer;
        bool back;
        Vector3D backVec;
    public:
        size_t runNormal;
        size_t runFast;
        float GetSpeed();
        void SetRespowPosition(const Vector3D& pos);
        void Init()override;
        void UniqueUpdate() override;
        void Move();
        void OnCollision(const CollisionInfomation& info) override;
        
    };

}

