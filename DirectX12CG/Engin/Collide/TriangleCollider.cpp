#include "TriangleCollider.h"

void MCB::TriangleCollider::Update()
{
	DirectX::XMMATRIX mat = object3d->GetMatWorld();
	Vector3D position;
	position.vec.x = mat.r[3].m128_f32[0] + offset.vec.x;
	position.vec.y = mat.r[3].m128_f32[1] + offset.vec.y;
	position.vec.z = mat.r[3].m128_f32[2] + offset.vec.z;
	Triangle::vertexPoint[0] = position + vertexPoint[0];
	Triangle::vertexPoint[1] = position + vertexPoint[1];
	Triangle::vertexPoint[2] = position + vertexPoint[2];
	Triangle::NormalCalculation();
	GetObject3D()->hited = false;
}
