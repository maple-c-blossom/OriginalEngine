#pragma once
#include "IgnoreWarning.h"
#include "pading.h"
#include "ICamera.h"
#include "Object3d.h"
#include "Vecter2D.h"
#include "Input.h"
namespace MCB
{
	class Camera :public ICamera
	{
	public:
		void Inilialize()override;
		void Update()override;


		void WorldPositionUpdate(const DirectX::XMMATRIX& playerMatrix,
			const DirectX::XMFLOAT3& playerPosition, bool isBillBord);

		void WorldPositionInit();

		DirectX::XMFLOAT3 Transform(const DirectX::XMFLOAT3& forward,const WorldMatrix& matworld);

		//ワールド変換行列
		DirectX::XMMATRIX GetMadWorld();

		//WorldMatrix GetMadWorld2();
		void SetCameraTarget(Object3d* target);
		 
		Camera(const Camera&) = delete;
		Camera();
		Camera& MCB::Camera::operator=(const Camera&) = delete;

		Vector2D rotAngle_;
		Vector2D cursorSpd_;
		Vector3D eye_;
		Vector3D target_;

		float disEyeTarget_ = 3.5f;
	private:

		std::unique_ptr<Object3d> object3d_ = std::make_unique<Object3d>();

		//Object3d* object3d = nullptr;
		Object3d* targetObj_;

 
		DirectX::XMFLOAT3 eyeStartPos_;
		Byte4 pad1;
		//プレイヤーのワールド変換行列
		DirectX::XMMATRIX playerMatrix_;

		 
		//レールカメラの初期座標
		DirectX::XMFLOAT3 firstPos_ = { 0,0,0 };


		DirectX::XMFLOAT3 angle_ = { 0,0,0 };

		MCB::Vector3D directVec_ = { 0,0,0 };
		Byte6 pad2;
		Byte6 pad3;
	};
}

