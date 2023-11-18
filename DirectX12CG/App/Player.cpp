#include "Player.h"
#include "Input.h"
#include "CollisionManager.h"
#include "PlayerQueryCallBack.h"
#include "AnimationModel.h"
#include "Sound.h"
#include "ImGuiManager.h"


using namespace std;
float MCB::Player::GetSpeed()
{
	return speedFront_;

}
void MCB::Player::SetRespowPosition(const Vector3D& pos)
{
	respownPosition_ = pos;
}
void MCB::Player::Init()
{
	Object3d::Init();
	scale_ = { 0.01f,0.01f,0.01f };
	SetCollider(make_unique<SphereCollider>(Vector3D{0,0.5f,0},0.5f));
	collider_->SetAttribute(ATTRIBUTE_FLENDRY);
	UpdateMatrix();
	collider_->Update();
	//position_ = { 0,0,-50 };
	respownPosition_ = position_;
	rotation_.y = ConvertRadius(180);
	rotation_.x = ConvertRadius(0);

}

void MCB::Player::UniqueUpdate()
{
	if ( !isDebug_ )
	{
		if ( !back )
		{
			Move();
		}
	}

	Object3d::UpdateMatrix();
	SphereCollider* sphere = dynamic_cast<SphereCollider*>(collider_);
	assert(sphere);

	if ( !isClimb )
	{
		Ray ray;
		ray.StartPosition_ = sphere->centerPosition_;
		ray.StartPosition_.vec_.y_ += sphere->GetRaius();
		ray.rayVec_ = { {{0,-1,0,0}} };
		RayCastHit info;
		float distRange = sphere->GetRaius() * 2.0f;
		float distOverRange = distRange * distoffSet;
		bool isGround = CollisionManager::GetInstance()->Raycast(ray,ATTRIBUTE_LANDSHAPE,&info,distRange + distOverRange);
		if ( isGraund_ )
		{
			//const float absDistance = 0.2f;

			if ( isGround )
			{
				isGraund_ = true;
				if ( info.dist_ <= 1.f - distOverRange ) position_.y -= ( ( info.dist_ ) - sphere->GetRaius() * 2.0f );
				Object3d::UpdateMatrix();
				for ( uint8_t i = 0; i < footBoneName.size(); i++ )
				{
					animationModel_->skeleton.GetNode(footBoneName[ i ])->ikData.isCollisionIk = true;
					//animationModel_->skeleton.SetTwoBoneIK();
				}
			}
			else
			{
				isGraund_ = false;
				fallV_ = { {{0,0,0,0}} };
			}
		}
		else if ( fallV_.vec_.y_ <= 0.0f )
		{

			if ( isGround )
			{
				isGraund_ = true;
				if ( info.dist_ <= 1.f - distOverRange ) position_.y -= ( ( info.dist_ - distOverRange ) - sphere->GetRaius() * 2.0f );
				Object3d::UpdateMatrix();
			}
			else
			{
				isGraund_ = false;
			}
		}
	}

	PlayerQueryCallBack callback(sphere);

	CollisionManager::GetInstance()->QuerySphere(*sphere, &callback, ATTRIBUTE_LANDSHAPE);

	if (callback.move.V3Len() >= distoffSet && !isClimb)
	{
		position_.x += callback.move.vec_.x_ * 0.4f;
		position_.y += callback.move.vec_.y_ * 0.4f;
		position_.z += callback.move.vec_.z_ * 0.4f;
	}

	//if (back)
	//{
	//	backTimer.SafeUpdate();
	//	position_.x += backVec.vec_.x_;
	//	position_.y += backVec.vec_.y_;
	//	position_.z += backVec.vec_.z_;
	//	if (backTimer.IsEnd())
	//	{
	//		back = false;
	//	}
	//}
	//else
	//{
	//	backTimer.Set(5);
	//	backVec = callback.move * 6;
	//	back = callback.block;
	//}

	if (position_.y < outYPosition)
	{
		position_ = respownPosition_.ConvertXMFloat3();
	}
}


void MCB::Player::Move()
{
	//SoundManager* sm = SoundManager::GetInstance();
	if (!input_->IsKeyDown(DIK_S) && !input_->IsKeyDown(DIK_W) && 
		!input_->gamePad_->RTrriger_.x_&& !input_->gamePad_->LTrriger_.x_)
	{
		speedFront_ = defualtSpeed_;
	}

	if (input_->IsKeyDown(DIK_W))
	{
		speedFront_ += acceleratorfront_;
	}

	if (input_->IsKeyDown(DIK_S))
	{
		speedFront_ -= acceleratorfront_;
	}

	if (input_->gamePad_->RTrriger_.x_)
	{
		accelerator_ *= input_->gamePad_->RTrriger_.x_;
		speedFront_ += acceleratorfront_;
	}
	else if(input_->gamePad_->LTrriger_.x_)
	{
		accelerator_ *= input_->gamePad_->LTrriger_.x_;
		speedFront_ -= acceleratorfront_;
	}

	speedFront_ = clamp(speedFront_, 0.f, maxFrontSpeed_);




	if (input_->IsKeyDown(DIK_D))
	{
		if (speedRight_ <= maxspeed_)speedRight_ += accelerator_;
		else speedRight_ = maxspeed_;
	}

	if (input_->IsKeyDown(DIK_A))
	{
		if (speedRight_ >= -maxspeed_)speedRight_ -= accelerator_;
		else speedRight_ = -maxspeed_;
	}

	if (input_->gamePad_->LStick_.x_)
	{
		float accelerator = maxspeed_;
		accelerator *= input_->gamePad_->LStick_.x_;
		if (speedRight_ <= maxspeed_ && speedRight_ >= -maxspeed_)speedRight_ += accelerator;
		else if (speedRight_ >= maxspeed_) speedRight_ = maxspeed_;
		else if (speedRight_ <= -maxspeed_)speedRight_ = -maxspeed_;
	}

	if (!input_->IsKeyDown(DIK_D) && !input_->IsKeyDown(DIK_A) && !input_->gamePad_->LStick_.x_)
	{
		speedRight_ = 0;
	}

	if (abs(speedFront_) > abs(speedRight_))
	{
		if (speedFront_ != 0)
		{
			if (speedFront_ > 0)currentAnimation_ = "Run";
			if (speedFront_ < 0)currentAnimation_ = "Run";

		}
	}
	else if(speedRight_ != 0)
	{
		if (speedRight_ > 0)currentAnimation_ = "Run";
		if (speedRight_ < 0)currentAnimation_ = "Run";
	}
	else
	{
		animationSpeed_ = 0.05f;
		currentAnimation_ = "Tpose";

	}
	animationSpeed_ = max(abs(speedFront_) / 7, abs(speedRight_) / 7);

	if (!isGraund_)
	{
		const float fallAcc = -0.025f;
		const float VYMin = -0.5f;
		fallV_.vec_.y_ = max(fallV_.vec_.y_ + fallAcc, VYMin);
		//animationPositionRock = true;
		//currentAnimation_ = "Jump";
		animationSpeed_ = 0.05f;

	}
	else if (Input::GetInstance()->IsKeyDown(DIK_SPACE) || input_->gamePad_->IsButtonDown(GAMEPAD_A))
	{
		isGraund_ = false;
		const float jumpVYFist = 0.45f;
		fallV_ = { {{0,jumpVYFist,0,0}} };
	}
	else if ( isGraund_ )
	{
		fallV_.vec_.y_ = 0;
		animationPositionRock = false;
	}
	wallCheckRay.StartPosition_ = position_;
	wallCheckRay.StartPosition_.vec_.y_ += 0.f;
	wallCheckRay.rayVec_ = nowFrontVec_;

	upperCheckRay.StartPosition_ = position_;
	upperCheckRay.StartPosition_.vec_.y_ += 1.5f;

	upperCheckRay.rayVec_ = nowFrontVec_;
	prevWallHit_ = wallHit_;
	RayCastHit info;
	wallHit_ = CollisionManager::GetInstance()->Raycast(wallCheckRay,ATTRIBUTE_WALL,&info,0.25f);
	//bool upperHit = CollisionManager::GetInstance()->Raycast(upperCheckRay,ATTRIBUTE_WALL,nullptr,0.15f);

	if ( info.objctPtr_ )
	{
		effectorPos.vec_.y_ = info.objctPtr_->position_.y + info.objctPtr_->scale_.y;
		effectorPos.vec_.z_ = info.objctPtr_->position_.z - info.objctPtr_->scale_.z;
	}

	if ( wallHit_ && !isClimb &&Input::GetInstance()->IsKeyDown(DIK_SPACE) )
	{
		fallV_.vec_.y_ = 0;
		position_.z = effectorPos.vec_.z_-0.025f;
		//  開始位置を保持
		climbOldPos = Vector3D(position_.x,effectorPos.vec_.y_ - 2.63f,effectorPos.vec_.z_ - 0.18f);
		position_.x = climbOldPos.vec_.x_;
		position_.y = climbOldPos.vec_.y_;
		position_.z = climbOldPos.vec_.z_;
		//  終了位置を算出
		climbPos = Vector3D(position_.x,effectorPos.vec_.y_,effectorPos.vec_.z_ + 0.15f);
		//  掴みを解除
		isGrab = false;
		//  よじ登りを実行
		isClimb = true;

		animeTime_ = 0;
		wallUPTimer.Set(60);

		poleVecLeft = Vector3D(position_.x - 1.15f,effectorPos.vec_.y_ + 0.5f,effectorPos.vec_.z_ - 2.f) ;
		poleVecRight = Vector3D(position_.x + 1.15f,effectorPos.vec_.y_ + 0.5f,effectorPos.vec_.z_ - 2.f) ;
		poleVecLF = Vector3D(position_.x - 1.15f,effectorPos.vec_.y_ - 1.5f,effectorPos.vec_.z_ + 2.f) ;
		poleVecRF = Vector3D(position_.x + 1.15f,effectorPos.vec_.y_ - 1.5f,effectorPos.vec_.z_ + 2.f) ;
		animationModel_->skeleton.GetNode("mixamorig:LeftHand")->lineView = true;
		animationModel_->skeleton.GetNode("mixamorig:RightHand")->lineView = true;
	}
	else if(wallHit_)
	{
		if (/* !prevWallHit_ ||*/isGraund_) 
		{
			wallTimer.Set(10);
		}
		wallTimer.Update();
		if ( !wallTimer.IsEnd() )
		{
			fallV_.vec_.y_ = 0;
			fallV_.vec_.y_ = speedFront_;
		}
	}

	if ( isClimb )
	{
		animationPositionRock = isRootStop;
		currentAnimation_ = "Climb";
		//currentAnimation_ = "Tpose";
		//animationModel_->skeleton.SetTwoBoneIK(*this,{ position_.x - 0.15f,effectorPos.vec_.y_,effectorPos.vec_.z_ },
		//	poleVecLeft,
		//	"mixamorig:LeftHand","NULL","NULL",false);
		//animationModel_->skeleton.SetTwoBoneIK(*this,{ position_.x + 0.15f ,effectorPos.vec_.y_,effectorPos.vec_.z_ },
		//	poleVecRight,
		//	"mixamorig:RightHand","NULL","NULL",false);

		//y0.7,z54.8;




		if ( !animationPositionRock )
		{
			position_.x = climbOldPos.vec_.x_;
			position_.y = climbOldPos.vec_.y_;
			position_.z = climbOldPos.vec_.z_;

		}
		wallUPTimer.Update(1);
		/*
		使ってるモーションの上がってる区間とそうじゃない区間がある？
		区間ごとにメモ取ってやるべきか？？？
		今日（11/18)できなかったら先生に相談。
		*/

		
		/*全体の動きの順
		* 1.足をかける動き->後方に移動
		* 2.上る動き->上に移動
		* 3.上る動き(速度変化)->上に移動
		* 4.前に行く(登りきる)->前に移動
		* 各時間
		* 1. 0.00～1.08
		* 2. 0.35～1.08
		* 3. 1.08～2.58
		* 4. 2.03～3.80※終了
		* 移動量
		* 1.
		* 2.
		* 3.
		* 4.
		*/


		int time = static_cast< int >( static_cast< float >( wallUPTimer.GetEndTime() ) * 0.73f );
		// 前後は後半にかけて早く移動する//前後も始まりずれてる。速度も少しずれている
		if ( time <= wallUPTimer.NowTime() && climbFrontMove)
		{
			
			position_.x = static_cast< float >( Lerp(climbOldPos.vec_.x_,climbPos.vec_.x_,
				wallUPTimer.GetEndTime() - time,wallUPTimer.NowTime() - time) );
			position_.z = static_cast< float >( Lerp(climbOldPos.vec_.z_,climbPos.vec_.z_,
				wallUPTimer.GetEndTime() - time,wallUPTimer.NowTime() - time) );
		}
		//  上下は等速直線で移動//上への動きが独り歩きしている（開始、速度全部ぐちゃぐちゃ）
		if ( time >= wallUPTimer.NowTime() && climbUpMove)
		{
			
			position_.y = static_cast< float >( Lerp(climbOldPos.vec_.y_,climbPos.vec_.y_,
				time,wallUPTimer.NowTime()) );
		}
		//currentAnimation->duration - 0.0001f
		//Animation* anim = animationModel_->animationManager.GetAnimation(currentAnimation_);
		animationSpeed_ = 3.8f / 60.f;
		//  座標を更新
		fallV_.vec_.y_ = 0.0f;
		//  進行度が8割を超えたらよじ登りの終了
		if ( wallUPTimer.IsEnd() )
		{
			isClimb = false;
			animationModel_->skeleton.TwoBoneIKOff("mixamorig:LeftHand");
			animationModel_->skeleton.TwoBoneIKOff("mixamorig:RightHand");
			position_.x = climbPos.vec_.x_;
			position_.y = climbPos.vec_.y_;
			position_.z = climbPos.vec_.z_;//最後に目的の場所に最終調整
		}
	}

	if ( !wallHit_ && !isClimb )
	{
		position_.x += nowFrontVec_.vec_.x_ * speedFront_;
		position_.z += nowFrontVec_.vec_.z_ * speedFront_;
		position_.x += rightVec_.vec_.x_ * speedRight_;
		position_.z += rightVec_.vec_.z_ * speedRight_;
	}

	position_.x += fallV_.vec_.x_;
	position_.y += fallV_.vec_.y_;
	position_.z += fallV_.vec_.z_;

}

void MCB::Player::Debug()
{
	ImGui::Checkbox("Debug",&isDebug_);
	ImGui::Checkbox("climbUpMove",&climbUpMove);
	ImGui::Checkbox("climbFrontMove",&climbFrontMove);
	ImGui::Checkbox("isRootStop",&isRootStop);
	if ( isDebug_ )
	{
		ImguiManager::GuizmoDraw(this,ImGuizmo::OPERATION::TRANSLATE,ImGuizmo::LOCAL);
		currentAnimation_ = "Climb";
		rotation_.y = ConvertRadius(180);
		animationSpeed_ = 0;
		animationPositionRock = isRootStop;
	}
	string rootName = animationModel_->animationManager.GetAnimation(currentAnimation_)->animationRootNode;
	Node* root = animationModel_->skeleton.GetNode(rootName);
	Vector3D nodePos;
	if ( root ) nodePos = root->worldPosition;
	ImGui::Text("Positin:%f,%f,%f",position_.x,position_.y,position_.z);
	ImGui::Text("EffectorPositin:%f,%f,%f",effectorPos.vec_.x_,effectorPos.vec_.y_,effectorPos.vec_.z_);
	ImGui::Text("ClimbStartPositin:%f,%f,%f",climbOldPos.vec_.x_,climbOldPos.vec_.y_,climbOldPos.vec_.z_);
	ImGui::Text("animationRootPos:%f,%f,%f",nodePos.vec_.x_,nodePos.vec_.y_,nodePos.vec_.z_);
	//position_.y = 2.88f;
	//position_.z = 55.96f;
	//0.73f;
	//local y2.48f,z0.13
	float animTime = animeTime_;
	ImGui::SliderFloat("AnimTime",&animTime,0.f,3.8f);
	animeTime_ = animTime;

}

void MCB::Player::OnCollision(const CollisionInfomation& info)
{
	if (info.object3d_->nameId_ == "checkPoint")
	{
		respownPosition_ = info.object3d_->position_;
	}

}


