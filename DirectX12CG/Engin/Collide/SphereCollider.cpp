#include "SphereCollider.h"

void MCB::SphereCollider::Update()
{
	DirectX::XMMATRIX mat = object3d->GetMatWorld();
	Sphere::centerPosition.vec.x = mat.r[3].m128_f32[0] + offset.vec.x;
	Sphere::centerPosition.vec.y = mat.r[3].m128_f32[1] + offset.vec.y;
	Sphere::centerPosition.vec.z = mat.r[3].m128_f32[2] + offset.vec.z;
	Sphere::radius = radius;
}
