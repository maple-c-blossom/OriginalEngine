#include "DebugCamera.h"
#include "Util.h"
#include "DirectXMath.h"
#include "DxWindow.h"
#include "Util.h"
using namespace MCB;
using namespace DirectX;

void MCB::DebugCamera::Inilialize()
{
	view.CreateMatrixView(XMFLOAT3(0.0f, 0.0f, -10.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
	projection.CreateMatrixProjection(XMConvertToRadians(45.0f), (float)DxWindow::GetInstance()->window_width / DxWindow::GetInstance()->window_height, 0.1f, 4000.0f);

	eye.vec.x = view.eye.x;
	eye.vec.y = view.eye.y;
	eye.vec.z = view.eye.z;
	target.vec.x = view.target.x;
	target.vec.y = view.target.y;
	target.vec.z = view.target.z;
	Vector3D frontVec = target - eye;
	disEyeTarget = frontVec.V3Len();
}

void MCB::DebugCamera::Update()
{
	if (input->IsKeyTrigger(DIK_R)) {
		rotAngle = Vector2D();
	}

#pragma region マウスの移動量
	Vector2D moveCursor((float)input->GetMousePosition().x,
		(float)input->GetMousePosition().y);
	float cursorDisPrev = moveCursor.GetLenge();
	moveCursor.VecNorm();
#pragma endregion

	eye.vec.x = view.eye.x;
	eye.vec.y = view.eye.y;
	eye.vec.z = view.eye.z;
	target.vec.x = view.target.x;
	target.vec.y = view.target.y;
	target.vec.z = view.target.z;
	Vector3D frontVec = target - eye;
	frontVec.V3Norm();
	Vector3D rightVec = Vector3D(0, 1, 0).GetV3Cross(frontVec);
	Vector3D downVec = rightVec.GetV3Cross(frontVec);
	rightVec.V3Norm();
	downVec.V3Norm();

	if (input->IsMouseDown(Input::MouseBotton::LEFT) && input->IsKeyDown(DIK_LSHIFT)) {
		if (cursorDisPrev != 0) {
			moveCursor /= 1000;
			moveCursor *= cursorDisPrev;
			if (view.up.y < 0) {
				moveCursor.x = -moveCursor.x;
			}
			rotAngle += moveCursor;
		}
	}

#pragma region 注視点と視点の距離変更
	if (input->IsKeyDown(DIK_LSHIFT)) {
		disEyeTarget += -input->GetMousePosition().z * (disEyeTarget * 0.001f);
	}
	if (disEyeTarget < 10) {
		disEyeTarget = 10;
	}
#pragma endregion

	target += rightVec * (float)(input->IsKeyDown(DIK_RIGHT) - input->IsKeyDown(DIK_LEFT));
	target += downVec * (float)(input->IsKeyDown(DIK_DOWN) - input->IsKeyDown(DIK_UP));
	target += -frontVec * ((float)input->IsKeyDown(DIK_N) - input->IsKeyDown(DIK_M));

	
	//if (rotAngle.x >= PI * 2) rotAngle.x -= PI * 2;
	//if (rotAngle.x < 0) rotAngle.x += PI * 2;
	//if (rotAngle.y >= PI * 2) rotAngle.y -= PI * 2;
	//if (rotAngle.y < 0) rotAngle.y += PI * 2;

	Vector2D angle = rotAngle;

#pragma region view代入
	view.up.y = cosf(angle.y);
	view.target.x = target.vec.x;
	view.target.y = target.vec.y;
	view.target.z = target.vec.z;
	view.eye.x = target.vec.x - disEyeTarget * cosf(angle.y) * sinf(angle.x);
	view.eye.y = target.vec.y + disEyeTarget * sinf(angle.y);
	view.eye.z = target.vec.z - disEyeTarget * cosf(angle.y) * cosf(angle.x);
#pragma endregion
}
