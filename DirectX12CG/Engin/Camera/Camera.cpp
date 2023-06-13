#include "Camera.h"
#include "DirectXMath.h"
#include "Util.h"
#include "DxWindow.h"
using namespace MCB;
using namespace DirectX;

void Camera::Inilialize()
{
	view.CreateMatrixView(XMFLOAT3(0.0f, 10.0f, -100.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
	projection.CreateMatrixProjection(XMConvertToRadians(45.0f), (float)DxWindow::GetInstance()->window_width / DxWindow::GetInstance()->window_height, 0.1f, 4000.0f);
}

void Camera::WorldPositionInit()
{
	object3d_->Init();
	object3d_->position = firstPos_;
	object3d_->rotasion = angle_;
	
}

void Camera::Update()
{
	
	view.UpDateMatrixView();
	projection.UpdataMatrixProjection();
}

void Camera::WorldPositionUpdate(DirectX::XMMATRIX playerMatrix, DirectX::XMFLOAT3 playerPosition,bool isBillBord)
{
	//object3d->Update(view,projection);

	object3d_->matWorld.SetMatScale(object3d_->scale.x, object3d_->scale.y, object3d_->scale.z);
	object3d_->matWorld.SetMatRot(object3d_->rotasion.x, object3d_->rotasion.y, object3d_->rotasion.z, false);
	object3d_->matWorld.SetMatTrans(object3d_->position.x, object3d_->position.y, object3d_->position.z);

	if (isBillBord)
	{
		if (object3d_->parent == nullptr)
		{
			object3d_->matWorld.UpdataBillBordMatrixWorld(view);
		}
		else
		{
			object3d_->matWorld.UpdataMatrixWorld();
		}
	}
	else
	{
		object3d_->matWorld.UpdataMatrixWorld();
	}

	if (object3d_->parent != nullptr)
	{
		object3d_->matWorld.matWorld *= object3d_->parent->matWorld.matWorld;
	}

	object3d_->GetConstMapTrans()->world = object3d_->matWorld.matWorld * view.mat;
	object3d_->GetConstMapTrans()->viewproj = projection.mat;
	object3d_->GetConstMapTrans()->cameraPos.x = view.eye.x;
	object3d_->GetConstMapTrans()->cameraPos.y = view.eye.y;
	object3d_->GetConstMapTrans()->cameraPos.z = view.eye.z;

	/*object3d->constMapTranceform->cameraPos.x = playerPosition.x;
	object3d->constMapTranceform->cameraPos.y = playerPosition.y;
	object3d->constMapTranceform->cameraPos.z = playerPosition.z;*/


}


XMMATRIX Camera::GetMadWorld()
{
	//ワールド座標を入れる変数
	XMMATRIX matWorld;
	matWorld = object3d_->matWorld.matWorld;

	return matWorld;
}


//void MCB::Camera::SetPlayerMatrix(DirectX::XMMATRIX playerMatrix)
//{
//	playerMatrix_ = playerMatrix;
//}

////
//WorldMatrix Camera::GetMadWorld2()
//{
//	//ワールド座標を入れる変数
//	WorldMatrix matWorld;
//	matWorld = object3d->matWorld;
//
//	return matWorld;
//}

XMFLOAT3 Camera::Transform(DirectX::XMFLOAT3 forward, WorldMatrix matWorld)
{
	XMFLOAT3 resultVec;

	resultVec.x = (forward.x * matWorld.matWorld.r[0].m128_f32[0]) + (forward.y * matWorld.matWorld.r[1].m128_f32[0]) + (forward.z * matWorld.matWorld.r[2].m128_f32[0]);
	resultVec.y = (forward.x * matWorld.matWorld.r[0].m128_f32[1]) + (forward.y * matWorld.matWorld.r[1].m128_f32[1]) + (forward.z * matWorld.matWorld.r[2].m128_f32[1]);
	resultVec.z = (forward.x * matWorld.matWorld.r[0].m128_f32[2]) + (forward.y * matWorld.matWorld.r[1].m128_f32[2]) + (forward.z * matWorld.matWorld.r[2].m128_f32[2]);

	return resultVec;
}



