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
		bool isGraund_ = false;
		Byte3 pad;
		Vector3D fallV_;
		Input* input_ = Input::GetInstance();
		float speedFront_ = 0.0f;
		float speedRight_ = 0.0f;
		float rotasionSpeed_ = 0.015f;
		float defualtSpeed_ = 0.05f;
		float maxFrontSpeed_ = 0.15f;
		float maxspeed_ = 0.35f;
		float accelerator_ = 0.005f;
		float acceleratorfront_ = 0.05f;
		MCB::Vector3D rightVec_ = { 1,0,0 };
		int32_t uptime = 300;
		Object3d* ground;
		Vector3D respownPosition_;
		float outYPosition = -20;
		float distoffSet = 0.00025f;
		Timer backTimer;
		bool back;
		bool isGrab;
		bool prevWallHit_;
		bool wallHit_;
		bool isDebug_ = false;
		bool climbUpMove = true;
		bool climbFrontMove = true;
		bool isRootStop = true;
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
		bool isIkClimb = true;
		uint8_t animationNum = 0;
		bool isJump;
		Vector3D poleVecLeft;
		Vector3D poleVecRight;
		Vector3D poleVecRF;
		Vector3D poleVecLF;
		
		std::array<std::string,2> footBoneName = { { "mixamorig:LeftFoot", "mixamorig:RightFoot"} };
		std::array<std::string,5> animationName = { { "Idle", "Run","Jump","Climb","Tpose"}};

	public:
		size_t runNormal;
		size_t runFast;
		float GetSpeed();
		void SetRespowPosition(const Vector3D& pos);
		void Init()override;
		void UniqueUpdate() override;
		void Move();
		void Debug();
		void OnCollision(const CollisionInfomation& info) override;

	};

}

