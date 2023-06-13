#include "WorldMatrix.h"

using namespace MCB;
using  namespace DirectX;

void WorldMatrix::SetMatRotX(const float& angle, const bool& ConvertToRadians)
{
	if (ConvertToRadians)
	{
		matRot_ *= XMMatrixRotationX(XMConvertToRadians(angle));
	}
	else
	{
		matRot_ *= XMMatrixRotationX(angle);
	}

}

void WorldMatrix::SetMatRotY(const float& angle, const bool& ConvertToRadians)
{
	if (ConvertToRadians)
	{
		matRot_ *= XMMatrixRotationY(XMConvertToRadians(angle));
	}
	else
	{
		matRot_ *= XMMatrixRotationY(angle);
	}
}

void WorldMatrix::SetMatRotZ(const float& angle, const bool& ConvertToRadians)
{
	if (ConvertToRadians)
	{
		matRot_ *= XMMatrixRotationZ(XMConvertToRadians(angle));
	}
	else
	{
		matRot_ *= XMMatrixRotationZ(angle);
	}
}

void WorldMatrix::SetMatRot(const float& angleX, const float& angleY, const float& angleZ,const bool& ConvertToRadians)
{
	matRot_ = XMMatrixIdentity();
	if (ConvertToRadians)
	{
		matRot_ *= XMMatrixRotationZ(XMConvertToRadians(angleZ));
		matRot_ *= XMMatrixRotationX(XMConvertToRadians(angleX));
		matRot_ *= XMMatrixRotationY(XMConvertToRadians(angleY));
	}
	else
	{
		matRot_ *= XMMatrixRotationZ(angleZ);
		matRot_ *= XMMatrixRotationX(angleX);
		matRot_ *= XMMatrixRotationY(angleY);
	}
}

XMMATRIX WorldMatrix::ReturnMatRot(const XMMATRIX& matRot , const float& angleX, 
	const float& angleY, const float& angleZ, const bool& ConvertToRadians)
{
	if (ConvertToRadians)
	{
		matRot_ *= XMMatrixRotationZ(XMConvertToRadians(angleZ));
		matRot_ *= XMMatrixRotationX(XMConvertToRadians(angleX));
		matRot_ *= XMMatrixRotationY(XMConvertToRadians(angleY));
	}
	else
	{
		matRot_ *= XMMatrixRotationZ(angleZ);
		matRot_ *= XMMatrixRotationX(angleX);
		matRot_ *= XMMatrixRotationY(angleY);
	}

	return matRot_;
}

void WorldMatrix::SetMatScale(const float& scaleX, const float& scaleY, const float& scaleZ)
{
	matScale_ = XMMatrixScaling(scaleX, scaleY, scaleZ);
}

void WorldMatrix::SetMatTrans(const float& transX, const float& transY, const float& transZ)
{
	matTransform_ = XMMatrixTranslation(transX, transY, transZ);
}


void WorldMatrix::CreateMatrixWorld(const XMMATRIX& matScale, const XMMATRIX& matRot, const XMMATRIX& matTransform)
{
	matScale_ = matScale;
	matRot_ = matRot;
	matTransform_ = matTransform;

	matWorld_ = XMMatrixIdentity();
	matWorld_ *= matScale;
	matWorld_ *= matRot;
	matWorld_ *= matTransform;
}

void WorldMatrix::UpdataMatrixWorld()
{
	matWorld_ = XMMatrixIdentity();
	matWorld_ *= matScale_;
	matWorld_ *= matRot_;
	matWorld_ *= matTransform_;
}

void WorldMatrix::UpdataBillBordMatrixWorld(const View& view)
{
	matWorld_ = XMMatrixIdentity();
	matWorld_ *= view.billMat_;
	matWorld_ *= matScale_;
	matWorld_ *= matRot_;
	matWorld_ *= matTransform_;
}

