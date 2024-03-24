#include "StartCamera.h"
#include "Util.h"
#include "DxWindow.h"
#include "RayCollider.h"
#include "CollisionManager.h"
#include "input.h"
WarningIgnoreBegin
#include "DirectXMath.h"
WarningIgnoreEnd
using namespace MCB;
using namespace DirectX;

void StartCamera::Inilialize()
{
	view_.CreateMatrixView(XMFLOAT3(0.0f,10.0f,-100.0f),XMFLOAT3(0.0f,0.0f,0.0f),XMFLOAT3(0.0f,1.0f,0.0f));
	projection_.CreateMatrixProjection(XMConvertToRadians(45.0f),static_cast< float >( DxWindow::GetInstance()->sWINDOW_WIDTH_ / DxWindow::GetInstance()->sWINDOW_HEIGHT_ ),0.1f,4000.0f);
	moveTimer.Set(390);
}

void StartCamera::WorldPositionInit()
{
	object3d_->Init();
	object3d_->position_ = firstPos_;
	object3d_->rotation_ = angle_;

}

void StartCamera::Update()
{

	if ( Input::GetInstance()->gamePad_->IsButtonDown(GAMEPAD::GAMEPAD_START) )
	{
		isSkip = true;
	}

	if ( targetstart_ && targetend_ )
	{
		moveTimer.SafeUpdate();
		if ( isSkip )
		{
			for(int i = 0; i < 3; i++)moveTimer.SafeUpdate();
		}


		if ( moveTimer.NowTime() >= moveTimer.GetEndTime() / 2 )
		{
			view_.eye_.y = static_cast< float >( InQuad(targetstart_->position_.y + 30.5f,targetend_->position_.y + 5.5f,
				moveTimer.GetEndTime() - moveTimer.GetEndTime() / 2,moveTimer.NowTime() - moveTimer.GetEndTime() / 2) );
			view_.target_.y = static_cast< float >( InQuad(targetstart_->position_.y,targetend_->position_.y,
				moveTimer.GetEndTime() - moveTimer.GetEndTime() / 2,moveTimer.NowTime() - moveTimer.GetEndTime() / 2) );

			view_.eye_.x = targetend_->position_.x;
			view_.eye_.z = targetend_->position_.z + ( 10.5f * -targetend_->nowFrontVec_.vec_.z_ );

			view_.target_.x = targetend_->position_.x;
			view_.target_.z = targetend_->position_.z;
		}
		else
		{
			view_.eye_.y = static_cast< float >( InQuad(targetstart_->position_.y + 30.5f,targetend_->position_.y + 5.5f,
				moveTimer.GetEndTime(),0) );
			view_.target_.y = static_cast< float >( InQuad(targetstart_->position_.y,targetend_->position_.y,
				moveTimer.GetEndTime(),0) );

			view_.eye_.x = static_cast< float >( InQuad(targetstart_->position_.x,targetend_->position_.x,moveTimer.GetEndTime() / 2,moveTimer.NowTime()) );
			view_.eye_.z = static_cast< float >( InQuad(targetstart_->position_.z + ( 10.5f * -targetstart_->nowFrontVec_.vec_.z_ ),
				targetend_->position_.z + ( 10.5f * -targetend_->nowFrontVec_.vec_.z_ ),moveTimer.GetEndTime() / 2,moveTimer.NowTime()) );

			view_.target_.x = static_cast< float >( InQuad(targetstart_->position_.x,targetend_->position_.x,
				moveTimer.GetEndTime() / 2,moveTimer.NowTime()) );
			view_.target_.z = static_cast< float >( InQuad(targetstart_->position_.z,targetend_->position_.z,
				moveTimer.GetEndTime() / 2,moveTimer.NowTime()) );
		}
	}
	view_.UpDateMatrixView();
	projection_.UpdataMatrixProjection();
}

void StartCamera::WorldPositionUpdate(const DirectX::XMMATRIX& playerMatrix,
	const DirectX::XMFLOAT3& playerPosition
	,bool isBillBord)
{
	//object3d->Update(view,projection);
	static_cast< void >( playerMatrix );
	static_cast< void >( playerPosition );
	object3d_->matWorld_.SetMatScale(object3d_->scale_.x,object3d_->scale_.y,object3d_->scale_.z);
	object3d_->matWorld_.SetMatRot(object3d_->rotation_.x,object3d_->rotation_.y,object3d_->rotation_.z,false);
	object3d_->matWorld_.SetMatTrans(object3d_->position_.x,object3d_->position_.y,object3d_->position_.z);

	if ( isBillBord )
	{
		if ( object3d_->parent_ == nullptr )
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

	if ( object3d_->parent_ != nullptr )
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


XMMATRIX StartCamera::GetMadWorld()
{
	//ワールド座標を入れる変数
	XMMATRIX matWorld;
	matWorld = object3d_->matWorld_.matWorld_;

	return matWorld;
}

void MCB::StartCamera::SetCameraTarget(Object3d* target)
{
	//target_ = target;
}

MCB::StartCamera::StartCamera()
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

XMFLOAT3 StartCamera::Transform(const DirectX::XMFLOAT3& forward,const WorldMatrix& matWorld)
{
	XMFLOAT3 resultVec;

	resultVec.x = ( forward.x * matWorld.matWorld_.r[ 0 ].m128_f32[ 0 ] ) + ( forward.y * matWorld.matWorld_.r[ 1 ].m128_f32[ 0 ] ) + ( forward.z * matWorld.matWorld_.r[ 2 ].m128_f32[ 0 ] );
	resultVec.y = ( forward.x * matWorld.matWorld_.r[ 0 ].m128_f32[ 1 ] ) + ( forward.y * matWorld.matWorld_.r[ 1 ].m128_f32[ 1 ] ) + ( forward.z * matWorld.matWorld_.r[ 2 ].m128_f32[ 1 ] );
	resultVec.z = ( forward.x * matWorld.matWorld_.r[ 0 ].m128_f32[ 2 ] ) + ( forward.y * matWorld.matWorld_.r[ 1 ].m128_f32[ 2 ] ) + ( forward.z * matWorld.matWorld_.r[ 2 ].m128_f32[ 2 ] );

	return resultVec;
}



