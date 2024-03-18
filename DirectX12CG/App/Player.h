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
		enum Catch
		{
			LH = 0,
			RH = 1,
			LF = 2,
			RF = 3
		};
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
		Timer moveTimer;
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

		std::array<Vector3D,4> poleVec;
		Vector3D climbMove;
		//Vector3D moveEffectorLeftHand;
		//Vector3D moveEffectorRightHand;
		Vector3D oldMovePos;
		Timer jumpokTimer;
		std::array<std::string,4> boneName = { {"mixamorig:LeftHand","mixamorig:RightHand",
			"mixamorig:LeftFoot", "mixamorig:RightFoot"} };
		std::array<std::string,5> animationName = { { "Idle", "Run","Jump","Climb","Tpose"}};

		bool upOk;
		bool developmentMode;

		float distanceCasterLight_ = 0.f;
		Float3 casterPos_ = { 0,0,0 };
		Float2 factorAngleCos_ = { 1.1f,0.9f };
		Float3 atten_ = { 0.0f,1.5f,0.2f };

		std::array<Object3d,4> moveEffectors;
		std::array<float,4> effectorMove;
		std::array<Sphere,4> grabHit;

		Object3d* climbObj;
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
		void Draw() override;
	};

}

