#include "View.h"
#include <DirectXMath.h>

using namespace DirectX;
using namespace MCB;

void View::CreateMatrixView(const XMFLOAT3& eye, const XMFLOAT3& target, const XMFLOAT3& up)
{
	eye_ = eye;
	target_ = target;
	up_ = up;
	mat_ = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

void View::UpDateMatrixView( bool billbordY)
{
	//mat = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	XMVECTOR eyePosition = XMLoadFloat3(&eye_);
	XMVECTOR targetPosition = XMLoadFloat3(&target_);
	XMVECTOR upVector = XMLoadFloat3(&up_);

	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);
	
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));

	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	XMVECTOR cameraAxisX;
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	XMVECTOR cameraAxisY;
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);
	cameraAxisY = XMVector3Normalize(cameraAxisY);

	XMMATRIX matCameraRot;
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);

	mat_ = XMMatrixTranspose(matCameraRot);

	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);

	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);
	mat_.r[3] = translation;

	if (billbordY)
	{
		XMVECTOR yBillAxisX, yBillAxisY, yBillAxisZ;
		yBillAxisX = cameraAxisX;
		yBillAxisY = XMVector3Normalize(XMVECTOR{up_.x, up_.y, up_.z});
		yBillAxisZ = XMVector3Cross(yBillAxisX, yBillAxisY);

		billMat_.r[0] = yBillAxisX;
		billMat_.r[1] = yBillAxisY;
		billMat_.r[2] = yBillAxisZ;
		billMat_.r[3] = XMVectorSet(0, 0, 0, 1);
	}
	else
	{
		billMat_.r[0] = cameraAxisX;
		billMat_.r[1] = cameraAxisY;
		billMat_.r[2] = cameraAxisZ;
		billMat_.r[3] = XMVectorSet(0, 0, 0, 1);
	}
}
