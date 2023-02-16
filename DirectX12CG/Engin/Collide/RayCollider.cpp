#include "RayCollider.h"

void MCB::RayCollider::Update()
{
	DirectX::XMMATRIX mat = object3d->GetMatWorld();
	Ray::StartPosition.vec.x = mat.r[3].m128_f32[0] + offset.vec.x;
	Ray::StartPosition.vec.y = mat.r[3].m128_f32[1] + offset.vec.y;
	Ray::StartPosition.vec.z = mat.r[3].m128_f32[2] + offset.vec.z;
	Ray::rayVec.vec.x = rayVec.vec.x;
	Ray::rayVec.vec.y = rayVec.vec.y;
	Ray::rayVec.vec.z = rayVec.vec.z;
	Ray::radius = radius;
	Ray::range = range;
	Ray::rayCasted = false;
	GetObject3D()->hited = false;
}
