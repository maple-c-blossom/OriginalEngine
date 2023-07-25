#pragma once
#include "Object3d.h"
#include "SphereCollider.h"
#include "Input.h"
#include "Particle.h"
#include "vector"
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
        float speed_ = 0.0125f;
        float srowspeed_ = 0.0125f;
        float maxspeed_ = 0.25f;
        MCB::Vector3D rightVec_ = {1,0,0};
        Object3d* ground;


    public:
        void Init();
        void UniqueUpdate() override;
        void Move();
        void OnCollision(const CollisionInfomation& info) override { hited_ = true; };
  
    };

}

