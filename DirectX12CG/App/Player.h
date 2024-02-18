#pragma once
#include "Object3d.h"
#include "SphereCollider.h"
#include "Input.h"
#include "Particle.h"
#include "vector"
#include "Timer.h"
namespace MCB
{
	class MoveBlock;
	class Player :public Object3d
	{

	private:
		bool isGraund_ = false;
		bool animePlay = false;
		bool isMoveBlock = false;
		bool isRespown_ = false;
		Vector3D fallV_;
		Input* input_ = Input::GetInstance();
		float speedFront_ = 0.0f;
		float speedRight_ = 0.0f;
		float rotasionSpeed_ = 0.015f;
		float defualtSpeed_ = 0.05f;
		float maxFrontSpeed_ = 0.25f;
		float maxspeed_ = 0.25f;
		float accelerator_ = 0.005f;
		float acceleratorfront_ = 0.05f;
		float animationPlaySpeed;
		Vector3D moveBlockTotal;

		int32_t uptime = 75;
		bool moving;
		Byte3 pad2;
		Object3d* ground;
		MoveBlock* moveBlock;
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
		bool prevIsClimb;
		bool isClimbUp;
		bool isIkClimb = true;
		uint8_t animationNum = 0;
		bool isJump;
		int climbMoveCount = 0;
		Vector3D directionVec;
		Vector3D poleVecLeft;
		Vector3D poleVecRight;
		Vector3D poleVecRF;
		Vector3D poleVecLF;
		Vector3D climbMove;
		Timer jumpokTimer;
		std::array<std::string,2> footBoneName = { { "mixamorig:LeftFoot", "mixamorig:RightFoot"} };
		std::array<std::string,5> animationName = { { "Idle", "Run","Jump","Climb","Tpose"}};



		float distanceCasterLight_ = 0.f;
		Float3 casterPos_ = { 0,0,0 };
		Float2 factorAngleCos_ = { 1.1f,0.9f };
		Float3 atten_ = { 0.0f,1.5f,0.2f };
	public:
		size_t runNormal;
		size_t runFast;
		int64_t pad3;
		float GetSpeed();
		void SetRespowPosition(const Vector3D& pos);
		void Init()override;
		void UniqueUpdate() override;
		void Move();
		void Debug();
		void OnCollision(const CollisionInfomation& info) override;
		void AnimationUpdate(bool isBillBord = false) override;
	};

}

