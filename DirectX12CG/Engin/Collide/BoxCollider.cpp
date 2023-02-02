#include "BoxCollider.h"
void MCB::BoxCollider::Update()
{
	DirectX::XMMATRIX mat = object3d->GetMatWorld();
	Box::position.x = mat.r[3].m128_f32[0] + offset.vec.x;
	Box::position.y = mat.r[3].m128_f32[1] + offset.vec.y;
	Box::position.z = mat.r[3].m128_f32[2] + offset.vec.z;
	Box::size = size;
	GetObject3D()->hited = false;
}
