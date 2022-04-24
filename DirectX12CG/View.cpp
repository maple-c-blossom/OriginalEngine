#include "View.h"

void View::CreateMatrixView(XMFLOAT3 eye, XMFLOAT3 target, XMFLOAT3 up)
{
	this->eye = eye;
	this->target = target;
	this->up = up;
	mat = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

void View::UpDateMatrixView()
{
	mat = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}
