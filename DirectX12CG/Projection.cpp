#include "Projection.h"

using namespace DirectX;
using namespace MCB;

void Projection::CreateMatrixProjection(float fovAngle, float aspectRatio, float nearZ, float farZ)
{
    this->fovAngle = fovAngle;
    this->aspectRatio = aspectRatio;
    this->nearZ = nearZ;
    this->farZ = farZ;

    mat = XMMatrixPerspectiveFovLH(
        fovAngle,
        aspectRatio,
        nearZ, farZ
    );
}

void Projection::UpdataMatrixProjection()
{
    mat = XMMatrixPerspectiveFovLH(
        fovAngle,
        aspectRatio,
        nearZ, farZ
    );
}
