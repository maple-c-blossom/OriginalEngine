#include "Projection.h"

using namespace DirectX;
using namespace MCB;

void Projection::CreateMatrixProjection(const float& fovAngle, const float& aspectRatio, const float& nearZ, const float& farZ)
{
    fovAngle_ = fovAngle;
    aspectRatio_ = aspectRatio;
    nearZ_ = nearZ;
    farZ_ = farZ;

    mat_ = XMMatrixPerspectiveFovLH(
        fovAngle,
        aspectRatio,
        nearZ, farZ
    );
}

void Projection::UpdataMatrixProjection()
{
    mat_ = XMMatrixPerspectiveFovLH(
        fovAngle_,
        aspectRatio_,
        nearZ_, farZ_
    );
}
