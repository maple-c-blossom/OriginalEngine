#include "DebugCamera.h"
#include "Util.h"
#include "DirectXMath.h"
#include "DxWindow.h"
#include "Util.h"
using namespace MCB;
using namespace DirectX;

void MCB::DebugCamera::Inilialize()
{
	view_.CreateMatrixView(XMFLOAT3(0.0f, 10.0f, -10.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
	projection_.CreateMatrixProjection(XMConvertToRadians(45.0f), (float)DxWindow::GetInstance()->sWINDOW_WIDTH_ / DxWindow::GetInstance()->sWINDOW_HEIGHT_, 0.1f, 4000.0f);

	eye_.vec_.x_ = view_.eye_.x;
	eye_.vec_.y_ = view_.eye_.y;
	eye_.vec_.z_ = view_.eye_.z;
	target_.vec_.x_ = view_.target_.x;
	target_.vec_.y_ = view_.target_.y;
	target_.vec_.z_ = view_.target_.z;
	Vector3D frontVec = target_ - eye_;
	disEyeTarget_ = frontVec.V3Len();
}

void MCB::DebugCamera::Update()
{
	if (input_->IsKeyTrigger(DIK_R)) {
		rotAngle_ = Vector2D();
	}

#pragma region マウスの移動量
	Vector2D moveCursor((float)input_->GetMousePosition().x,
		(float)input_->GetMousePosition().y);
	float cursorDisPrev = moveCursor.GetLenge();
	moveCursor.VecNorm();
#pragma endregion

	eye_.vec_.x_ = view_.eye_.x;
	eye_.vec_.y_ = view_.eye_.y;
	eye_.vec_.z_ = view_.eye_.z;
	target_.vec_.x_ = view_.target_.x;
	target_.vec_.y_ = view_.target_.y;
	target_.vec_.z_ = view_.target_.z;
	Vector3D frontVec = target_ - eye_;
	frontVec.V3Norm();
	Vector3D rightVec = Vector3D(0, 1, 0).GetV3Cross(frontVec);
	Vector3D downVec = rightVec.GetV3Cross(frontVec);
	rightVec.V3Norm();
	downVec.V3Norm();

	if (input_->IsMouseDown(Input::MouseBotton::LEFT) && input_->IsKeyDown(DIK_LSHIFT)) {
		if (cursorDisPrev != 0) {
			moveCursor /= 1000;
			moveCursor *= cursorDisPrev;
			if (view_.up_.y < 0) {
				moveCursor.x_ = -moveCursor.x_;
			}
			rotAngle_ += moveCursor;
		}
	}

#pragma region 注視点と視点の距離変更
	if (input_->IsKeyDown(DIK_LSHIFT)) {
		disEyeTarget_ += -input_->GetMousePosition().z * (disEyeTarget_ * 0.001f);
	}
	if (disEyeTarget_ < 10) {
		disEyeTarget_ = 10;
	}
#pragma endregion

	target_ += rightVec * (float)(input_->IsKeyDown(DIK_RIGHT) - input_->IsKeyDown(DIK_LEFT));
	target_ += downVec * (float)(input_->IsKeyDown(DIK_DOWN) - input_->IsKeyDown(DIK_UP));
	target_ += -frontVec * ((float)input_->IsKeyDown(DIK_N) - input_->IsKeyDown(DIK_M));

	
	//if (rotAngle.x >= PI * 2) rotAngle.x -= PI * 2;
	//if (rotAngle.x < 0) rotAngle.x += PI * 2;
	//if (rotAngle.y >= PI * 2) rotAngle.y -= PI * 2;
	//if (rotAngle.y < 0) rotAngle.y += PI * 2;

	Vector2D angle = rotAngle_;

#pragma region view代入
	view_.up_.y = cosf(angle.y_);
	view_.target_.x = target_.vec_.x_;
	view_.target_.y = target_.vec_.y_;
	view_.target_.z = target_.vec_.z_;
	view_.eye_.x = target_.vec_.x_ - disEyeTarget_ * cosf(angle.y_) * sinf(angle.x_);
	view_.eye_.y = target_.vec_.y_ + disEyeTarget_ * sinf(angle.y_);
	view_.eye_.z = target_.vec_.z_ - disEyeTarget_ * cosf(angle.y_) * cosf(angle.x_);
	view_.UpDateMatrixView();
	projection_.UpdataMatrixProjection();

#pragma endregion
}
