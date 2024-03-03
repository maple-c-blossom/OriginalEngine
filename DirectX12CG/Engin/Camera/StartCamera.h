#pragma once
#include "ICamera.h"
#include "Timer.h"
namespace MCB
{
	class StartCamera :
		public ICamera
	{
	public:
		void Inilialize()override;
		void Update()override;


		void WorldPositionUpdate(const DirectX::XMMATRIX& playerMatrix,
			const DirectX::XMFLOAT3& playerPosition,bool isBillBord);

		void WorldPositionInit();

		DirectX::XMFLOAT3 Transform(const DirectX::XMFLOAT3& forward,const WorldMatrix& matworld);

		//ワールド変換行列
		DirectX::XMMATRIX GetMadWorld();

		//WorldMatrix GetMadWorld2();
		void SetCameraTarget(Object3d* target);

		StartCamera(const StartCamera&) = delete;
		StartCamera();
		StartCamera& MCB::StartCamera::operator=(const StartCamera&) = delete;

		Object3d* targetstart_;
		Object3d* targetend_;
		Timer moveTimer;
	private:
		bool isSkip = false;
		std::unique_ptr<Object3d> object3d_ = std::make_unique<Object3d>();

		//Object3d* object3d = nullptr;


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
