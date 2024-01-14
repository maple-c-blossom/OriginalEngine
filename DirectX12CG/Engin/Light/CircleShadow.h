#pragma once
#include "MCBFloat.h"
#include "Vector3D.h"
#include "MCBMatrix.h"

namespace MCB
{
	class CircleShadow
	{
	private:
		Vector3D dir_ = { 1,0,0 };
		float distanceCasterLight_ = 100.f;
		Float3 casterPos_ = { 0,0,0 };
		Float3 atten_ = { 0.5f,0.6f,0.f };
		Float2 factorAngleCos_ = { 0.2f,0.5f };
		bool active_ = false;
	public:
		struct ConstBufferData
		{
			Vector3D dir;
			float distanceCasterLight;
			Float3 casterPos;
			float pad1;
			Float3 atten;
			float pad2;
			Float2 factorAngleCos;
			unsigned int active;
			float pad3;
		};

		void SetDir(const Vector3D& dir);
		const Vector3D& GetDir();
		void SetCasterPos(const Float3& casterPos);
		const Float3& GetCasterPos();
		void SetDistanceCasterLight(float dist);
		const float GetDistCasterL();
		void SetAtten(const Float3& atten);
		const Float3& GetAtten();
		void SetFactorAngleCos(const Float2& factorAngle);
		const Float2& GetFacterAngleCos();
		void SetActive(bool active);
		const bool IsActive();
	};
}

