#pragma once
#include "ICamera.h"
#include "Object3d.h"

namespace MCB
{
	class Camera :public ICamera
	{
	public:
		void Inilialize()override;
		void Update()override;

		void WorldPositionUpdate(DirectX::XMMATRIX playerMatrix, DirectX::XMFLOAT3 playerPosition,bool isBillBord);

		void WorldPositionInit();

		DirectX::XMFLOAT3 Transform(DirectX::XMFLOAT3 forward, WorldMatrix matworld);

		//���[���h�ϊ��s��
		DirectX::XMMATRIX GetMadWorld();

		//WorldMatrix GetMadWorld2();


	private:

		std::unique_ptr<Object3d> object3d = std::make_unique<Object3d>();

		//Object3d* object3d = nullptr;


 
		DirectX::XMFLOAT3 eyeStartPos;

		//�v���C���[�̃��[���h�ϊ��s��
		DirectX::XMMATRIX playerMatrix_;
		 
		//���[���J�����̏������W
		DirectX::XMFLOAT3 firstPos = { 0,0,0 };

		//���[���J�����̏������W
		DirectX::XMFLOAT3 angle = { 0,0,0 };

		MCB::Vector3D directVec = { 0,0,0 };

	};
}

