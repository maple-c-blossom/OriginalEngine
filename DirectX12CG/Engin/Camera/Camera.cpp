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
	disEyeTarget_ = 3.5f;
	rotAngle_ = { -0.0203672126f ,0.285243392f };
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

	

	if ( targetObj_->cameraViewFromSide_ )
	{
		view_.eye_.x = targetObj_->position_.x + ( 7.5f * -targetObj_->nowFrontVec_.vec_.x_ ) + 5.5f;
		view_.eye_.y = targetObj_->position_.y;
		view_.eye_.z = targetObj_->position_.z;
		view_.target_ = targetObj_->position_;
		view_.target_.y = view_.eye_.y;

	}
	else
	{
		Input* input =	Input::GetInstance();
		Vector2D moveCursor(( float ) input->GetMousePosition().x,
	( float ) input->GetMousePosition().y);
		float cursorDisPrev = moveCursor.GetLenge();
		moveCursor.VecNorm();
		eye_.vec_.x_ = view_.eye_.x;
		eye_.vec_.y_ = view_.eye_.y;
		eye_.vec_.z_ = view_.eye_.z;
		target_.vec_.x_ = targetObj_->position_.x;
		target_.vec_.y_ = targetObj_->position_.y;
		target_.vec_.z_ = targetObj_->position_.z;
		Vector3D frontVec = target_ - eye_;
		frontVec.V3Norm();
		Vector3D rightVec = Vector3D(0,1,0).GetV3Cross(frontVec);
		Vector3D downVec = rightVec.GetV3Cross(frontVec);
		rightVec.V3Norm();
		downVec.V3Norm();

		if ( input->gamePad_->RStick_.x_ || input->gamePad_->RStick_.y_ )
		{
			moveCursor = Vector2D(input->gamePad_->RStick_.x_ / 50.f,input->gamePad_->RStick_.y_ / 50.f);
			if ( view_.up_.y < 0 )
			{
				moveCursor.x_ = -input->gamePad_->RStick_.x_;
			}
			rotAngle_ += moveCursor;
		}


#pragma region 注視点と視点の距離変更
		if ( input->IsKeyDown(DIK_RSHIFT) )
		{
			disEyeTarget_ += static_cast< float >( -input->GetMousePosition().z ) * ( disEyeTarget_ * 0.001f );
		}
		//if (disEyeTarget_ < 10) {
		//	disEyeTarget_ = 4;
		//}
#pragma endregion

		float moveLen = 0;
		moveLen += ( ( ( float ) input->gamePad_->IsInputLTrriger().x_ - input->gamePad_->IsInputRTrriger().x_ ) * 0.25f );
		moveLen += ( ( ( float ) input->IsKeyDown(DIK_N) - input->IsKeyDown(DIK_M) ) * 0.25f );

		disEyeTarget_ += moveLen;
		if ( disEyeTarget_ < 2 )
		{
			disEyeTarget_ = 2;
		}
		if ( disEyeTarget_ > 20 )
		{
			disEyeTarget_ = 20;
		}

		//if (rotAngle_.x_ >= PI * 2) rotAngle_.x_ -= PI * 2;
		//if (rotAngle_.x_ < 0) rotAngle_.x_ += PI * 2;
		//if (rotAngle_.y >= PI * 2) rotAngle_.y -= PI * 2;
		//if (rotAngle_.y < 0) rotAngle_.y += PI * 2;

		Vector2D angle = rotAngle_;

#pragma region view_代入
		view_.up_.y = cosf(angle.y_);
		view_.target_.x = target_.vec_.x_;
		view_.target_.y = target_.vec_.y_;
		view_.target_.z = target_.vec_.z_;
		view_.eye_.x = target_.vec_.x_ - disEyeTarget_ * cosf(angle.y_) * sinf(angle.x_);
		view_.eye_.y = target_.vec_.y_ + disEyeTarget_ * sinf(angle.y_);
		view_.eye_.z = target_.vec_.z_ - disEyeTarget_ * cosf(angle.y_) * cosf(angle.x_);
	}




	Ray ray;
	ray.StartPosition_.vec_.x_ = view_.eye_.x;
	ray.StartPosition_.vec_.y_ = view_.eye_.y;
	ray.StartPosition_.vec_.z_ = view_.eye_.z;
	ray.rayVec_ = Vector3D().Vector3Substruct(ray.StartPosition_.vec_, Float3(targetObj_->position_.x,
		targetObj_->position_.y
		+ 0.5f ,
		targetObj_->position_.z));
	float rayLength = ray.rayVec_.V3Len();
	ray.rayVec_.V3Norm();
	RayCastHit info;
	if (CollisionManager::GetInstance()->Raycast(ray, ATTRIBUTE_LANDSHAPE, &info, rayLength - 0.5f))
	{
		if (info.objctPtr_ != nullptr)
		{
			if (info.objctPtr_ != targetObj_ )info.objctPtr_->isInvisible = true;
		}
	}

	targetObjctVec_ = Vector3D(eye_,target_);
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
	targetObj_ = target;
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



