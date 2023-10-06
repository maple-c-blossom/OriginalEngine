#pragma once
#include "Object3d.h"
#include "Timer.h"
#include "Util.h"
#include "ICamera.h"

namespace MCB
{
	class PopEffect :public MCB::Object3d
	{
		Timer lifeTimer_;
		float defaultSpeed_;
		float speed_;
		Float3 defaultScale_;
	public:
		bool deleteFlag_ = false;
		Byte3 pad;
		void Initialize(MCB::Model* model, MCB::Vector3D Velocity, MCB::Float3 position, MCB::Float3 scale, MCB::Float4 color, float speed, int lifeTime);
		void UniqueUpdate() override;
		void UpdateMatrix(MCB::ICamera* camera);
		void UpdateMatrix(bool isBillBord = false)override;
		void UpdateMatrix(Quaternion q,bool isBillBord = false) override;
		void Draw();
		void Draw(uint16_t)override;

	};
}

