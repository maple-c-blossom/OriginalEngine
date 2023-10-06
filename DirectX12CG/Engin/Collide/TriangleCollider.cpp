#include "TriangleCollider.h"
using namespace MCB;

MCB::TriangleCollider::TriangleCollider(const Vector3D& offset,const Vector3D& normal,const std::array<Vector3D,3>& vertexPoint)
{
	offset_ = offset,normal_ = normal,vertexPoint_ = vertexPoint;
	primitive_ = PrimitiveType::TRIANGLE;
};
void MCB::TriangleCollider::Update()
{
	DirectX::XMMATRIX mat = object3d_->GetMatWorld();
	Vector3D position;
	position.vec_.x_ = mat.r[3].m128_f32[0] + offset_.vec_.x_;
	position.vec_.y_ = mat.r[3].m128_f32[1] + offset_.vec_.y_;
	position.vec_.z_ = mat.r[3].m128_f32[2] + offset_.vec_.z_;
	Triangle::vertexPoint_[0] = position + vertexPoint_[0];
	Triangle::vertexPoint_[1] = position + vertexPoint_[1];
	Triangle::vertexPoint_[2] = position + vertexPoint_[2];
	Triangle::NormalCalculation();
	GetObject3D()->hited_ = false;
}

void MCB::TriangleCollider::SetVertexPoint(const Vector3D& point,size_t index)
{
	vertexPoint_[ index ] = point;
}
