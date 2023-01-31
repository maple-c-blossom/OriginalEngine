#include "Collider.h"
#include "Util.h"
using namespace MCB;

bool MCB::CalcSphere(Sphere sphereA, Sphere sphereB)
{

	float hitX = (sphereA.centerPosition.vec.x - sphereB.centerPosition.vec.x) * (sphereA.centerPosition.vec.x - sphereB.centerPosition.vec.x);
	float hitY = (sphereA.centerPosition.vec.y - sphereB.centerPosition.vec.y) * (sphereA.centerPosition.vec.y - sphereB.centerPosition.vec.y);
	float hitZ = (sphereA.centerPosition.vec.z - sphereB.centerPosition.vec.z) * (sphereA.centerPosition.vec.z - sphereB.centerPosition.vec.z);
	float hitR = (sphereB.radius + sphereA.radius) * (sphereB.radius + sphereA.radius);
	hitX = Abs(hitX);
	hitY = Abs(hitY);
	hitZ = Abs(hitZ);
	float hit = hitX + hitY + hitZ;
	if (hit <= hitR)
	{
		return true;
	}
	return false;
}

bool MCB::CalcPlaneSpher(Plane plane, Sphere sphere)
{
	float dist = sphere.centerPosition.GetV3Dot(plane.normal);
	dist = dist - plane.distance;
	if (fabsf(dist) > sphere.radius) return false;
	return true;
}

bool MCB::CalcTriangleSpher(Triangle plane, Sphere sphere)
{
	return false;
}

bool MCB::CalcTrianglePoint(Triangle triangle, Vector3D point, Vector3D& closest)
{
	//// �����̈�`�F�b�N�@�̑O����
	//Vector3D p0p1 = triangle.vertexPoint[1] - triangle.vertexPoint[0];
	//Vector3D p0p2 = triangle.vertexPoint[2] - triangle.vertexPoint[0];
	//Vector3D p0pt = point - triangle.vertexPoint[0];
	////�����̈�`�F�b�N�@
	//float d1 = p0p1.GetV3Dot(p0pt);
	//float d2 = p0p2.GetV3Dot(p0pt);

	//if (d1 <= 0.0f && d2 <= 0.0f)
	//{
	//	// p0���ŋߖT
	//	closest = triangle.vertexPoint[0];
	//	return;
	//}
	////-------------
	//// �����̈�`�F�b�N�A�̑O����
	//Vector3D p1pt = point - triangle.vertexPoint[1];

	//float d3 = XMVector3Dot(p0p1, p1pt);
	//float d4 = XMVector3Dot(p0p2, p1pt);

	//if (d3.m128_f32[0] >= 0.0f && d4.m128_f32[0] <= d3.m128_f32[0])
	//{
	//	// p1���ŋߖT
	//	*closest = triangle.p1;
	//	return;
	//}

	//// point��p0_p1�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p1��ɑ΂���ˉe��Ԃ�
	//float vc = d1.m128_f32[0] * d4.m128_f32[0] - d3.m128_f32[0] * d2.m128_f32[0];
	//if (vc <= 0.0f && d1.m128_f32[0] >= 0.0f && d3.m128_f32[0] <= 0.0f)
	//{
	//	float v = d1.m128_f32[0] / (d1.m128_f32[0] - d3.m128_f32[0]);
	//	*closest = triangle.p0 + v * p0_p1;
	//	return;
	//}

	//// point��p2�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	//XMVECTOR p2_pt = point - triangle.p2;

	//XMVECTOR d5 = XMVector3Dot(p0_p1, p2_pt);
	//XMVECTOR d6 = XMVector3Dot(p0_p2, p2_pt);
	//if (d6.m128_f32[0] >= 0.0f && d5.m128_f32[0] <= d6.m128_f32[0])
	//{
	//	*closest = triangle.p2;
	//	return;
	//}

	//// point��p0_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p2��ɑ΂���ˉe��Ԃ�
	//float vb = d5.m128_f32[0] * d2.m128_f32[0] - d1.m128_f32[0] * d6.m128_f32[0];
	//if (vb <= 0.0f && d2.m128_f32[0] >= 0.0f && d6.m128_f32[0] <= 0.0f)
	//{
	//	float w = d2.m128_f32[0] / (d2.m128_f32[0] - d6.m128_f32[0]);
	//	*closest = triangle.p0 + w * p0_p2;
	//	return;
	//}

	//// point��p1_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p1_p2��ɑ΂���ˉe��Ԃ�
	//float va = d3.m128_f32[0] * d6.m128_f32[0] - d5.m128_f32[0] * d4.m128_f32[0];
	//if (va <= 0.0f && (d4.m128_f32[0] - d3.m128_f32[0]) >= 0.0f && (d5.m128_f32[0] - d6.m128_f32[0]) >= 0.0f)
	//{
	//	float w = (d4.m128_f32[0] - d3.m128_f32[0]) / ((d4.m128_f32[0] - d3.m128_f32[0]) + (d5.m128_f32[0] - d6.m128_f32[0]));
	//	*closest = triangle.p1 + w * (triangle.p2 - triangle.p1);
	//	return;
	//}

	//float denom = 1.0f / (va + vb + vc);
	//float v = vb * denom;
	//float w = vc * denom;
	//*closest = triangle.p0 + p0_p1 * v + p0_p2 * w;
	return false;
}

bool MCB::CalcRaySphere(Ray ray, Sphere sphere)
{
	Vector3D rayToSphere(ray.StartPosition, sphere.centerPosition.vec);
	ray.rayVec.V3Norm();
	float tempPos = rayToSphere.GetV3Dot(ray.rayVec);
	ray.rayVec = ray.rayVec * tempPos;
	Float3 Temp;
	Temp.x = ray.rayVec.vec.x + ray.StartPosition.x;
	Temp.y = ray.rayVec.vec.y + ray.StartPosition.y;
	Temp.z = ray.rayVec.vec.z + ray.StartPosition.z;
	//�Փ˒n�_�����C�̎˒��𒴂��Ă��邩���C�̋t�����������ꍇ
	if (tempPos > ray.range || tempPos < 0)
	{
		return false;
	}
	Vector3D aVec;

	aVec.V3Get(sphere.centerPosition.vec, Temp);

	float a = aVec.V3Len();

	return a < sphere.radius + ray.radius;
}

