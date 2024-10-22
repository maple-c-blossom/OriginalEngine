#include "Camera.h"
#include "Util.h"
#include "DxWindow.h"
#include "RayCollider.h"
#include "CollisionManager.h"
WarningIgnoreBegin
#include "DirectXMath.h"
WarningIgnoreEnd
using namespace MCB;
using namespace DirectX;

void Camera::Inilialize()
{
	view_.CreateMatrixView(XMFLOAT3(0.0f, 10.0f, -100.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
	projection_.CreateMatrixProjection(XMConvertToRadians(45.0f), static_cast<float>(DxWindow::GetInstance()->sWINDOW_WIDTH_ / DxWindow::GetInstance()->sWINDOW_HEIGHT_), 0.1f, 4000.0f);

}

void Camera::WorldPositionInit()
{
	object3d_->Init();
	object3d_->position_ = firstPos_;
	object3d_->rotation_ = angle_;
	
}

void Camera::Update()
{
	Float3 frongtOffSet;//前後方向

	

	if ( target_->cameraViewFromSide_ )
	{
		view_.eye_.x = target_->position_.x + ( 7.5f * -target_->nowFrontVec_.vec_.x_ ) + 5.5f;
		view_.eye_.y = target_->position_.y;
		view_.eye_.z = target_->position_.z;
		view_.target_ = target_->position_;
		view_.target_.y = view_.eye_.y;

	}
	else
	{
		view_.eye_.x = target_->position_.x ;
		view_.eye_.y = target_->position_.y + 5.5f;
		view_.eye_.z = target_->position_.z + ( 10.5f * -target_->nowFrontVec_.vec_.z_ );
		view_.target_ = target_->position_;
	}




	Ray ray;
	ray.StartPosition_.vec_.x_ = view_.eye_.x;
	ray.StartPosition_.vec_.y_ = view_.eye_.y;
	ray.StartPosition_.vec_.z_ = view_.eye_.z;
	ray.rayVec_ = Vector3D().Vector3Substruct(ray.StartPosition_.vec_, Float3(target_->position_.x,
		target_->position_.y
		+ 0.5f ,
		target_->position_.z));
	float rayLength = ray.rayVec_.V3Len();
	ray.rayVec_.V3Norm();
	RayCastHit info;
	if (CollisionManager::GetInstance()->Raycast(ray, ATTRIBUTE_LANDSHAPE, &info, rayLength - 0.5f))
	{
		if (info.objctPtr_ != nullptr)
		{
			if (info.objctPtr_ != target_)info.objctPtr_->isInvisible = true;
		}
	}


	view_.UpDateMatrixView();
	projection_.UpdataMatrixProjection();
}

void Camera::WorldPositionUpdate(const DirectX::XMMATRIX& playerMatrix,
	const DirectX::XMFLOAT3& playerPosition
	, bool isBillBord)
{
	//object3d->Update(view,projection);
	static_cast<void>(playerMatrix);
	static_cast<void>(playerPosition);
	object3d_->matWorld_.SetMatScale(object3d_->scale_.x, object3d_->scale_.y, object3d_->scale_.z);
	object3d_->matWorld_.SetMatRot(object3d_->rotation_.x, object3d_->rotation_.y, object3d_->rotation_.z, false);
	object3d_->matWorld_.SetMatTrans(object3d_->position_.x, object3d_->position_.y, object3d_->position_.z);

	if (isBillBord)
	{
		if (object3d_->parent_ == nullptr)
		{
			object3d_->matWorld_.UpdataBillBordMatrixWorld(view_);
		}
		else
		{
			object3d_->matWorld_.UpdataMatrixWorld();
		}
	}
	else
	{
		object3d_->matWorld_.UpdataMatrixWorld();
	}

	if (object3d_->parent_ != nullptr)
	{
		object3d_->matWorld_.matWorld_ *= object3d_->parent_->matWorld_.matWorld_;
	}

	object3d_->GetConstMapTrans()->world = object3d_->matWorld_.matWorld_ * view_.mat_;
	object3d_->GetConstMapTrans()->viewproj = projection_.mat_;
	object3d_->GetConstMapTrans()->cameraPos.x_ = view_.eye_.x;
	object3d_->GetConstMapTrans()->cameraPos.y_ = view_.eye_.y;
	object3d_->GetConstMapTrans()->cameraPos.z_ = view_.eye_.z;

	/*object3d->constMapTranceform->cameraPos.x = playerPosition.x;
	object3d->constMapTranceform->cameraPos.y = playerPosition.y;
	object3d->constMapTranceform->cameraPos.z = playerPosition.z;*/


}


XMMATRIX Camera::GetMadWorld()
{
	//ワールド座標を入れる変数
	XMMATRIX matWorld;
	matWorld = object3d_->matWorld_.matWorld_;

	return matWorld;
}

void MCB::Camera::SetCameraTarget(Object3d* target)
{
	target_ = target;
}

MCB::Camera::Camera()
{
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

XMFLOAT3 Camera::Transform(const DirectX::XMFLOAT3& forward, const WorldMatrix& matWorld)
{
	XMFLOAT3 resultVec;

	resultVec.x = (forward.x * matWorld.matWorld_.r[0].m128_f32[0]) + (forward.y * matWorld.matWorld_.r[1].m128_f32[0]) + (forward.z * matWorld.matWorld_.r[2].m128_f32[0]);
	resultVec.y = (forward.x * matWorld.matWorld_.r[0].m128_f32[1]) + (forward.y * matWorld.matWorld_.r[1].m128_f32[1]) + (forward.z * matWorld.matWorld_.r[2].m128_f32[1]);
	resultVec.z = (forward.x * matWorld.matWorld_.r[0].m128_f32[2]) + (forward.y * matWorld.matWorld_.r[1].m128_f32[2]) + (forward.z * matWorld.matWorld_.r[2].m128_f32[2]);

	return resultVec;
}



