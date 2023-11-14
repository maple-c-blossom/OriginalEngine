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
		Byte3 pad;
		Vector3D fallV_;
		Input* input_ = Input::GetInstance();
		float speedFront_ = 0.0f;
		float speedRight_ = 0.0f;
		float rotasionSpeed_ = 0.015f;
		float defualtSpeed_ = 0.15f;
		float maxFrontSpeed_ = 0.35f;
		float maxspeed_ = 0.35f;
		float accelerator_ = 0.005f;
		float acceleratorfront_ = 0.05f;
		MCB::Vector3D rightVec_ = { 1,0,0 };
		Byte4 pad2;
		Object3d* ground;
		Vector3D respownPosition_;
		float outYPosition = -20;
		float distoffSet = 0.00025f;
		Timer backTimer;
		bool back;
		bool isGrab;
		bool prevWallHit_;
		bool wallHit_;
		Ray wallCheckRay;
		Ray upperCheckRay;
		Timer wallUPTimer;
		Timer wallTimer;
		float wallCheckOffSet;
		float upperCheckOffSet;
		Vector3D backVec;
		Vector3D climbOldPos;
		Vector3D climbPos;
		Vector3D effectorPos;
		bool isClimb;
		Byte7 pad4;
		Vector3D poleVecLeft;
		Vector3D poleVecRight;
		Vector3D poleVecRF;
		Vector3D poleVecLF;

		std::array<std::string,2> footBoneName = { { "mixamorig:LeftFoot", "mixamorig:RightFoot"} };
	public:
		size_t runNormal;
		size_t runFast;
		int64_t pad5;
		float GetSpeed();
		void SetRespowPosition(const Vector3D& pos);
		void Init()override;
		void UniqueUpdate() override;
		void Move();
		void OnCollision(const CollisionInfomation& info) override;

	};

}

