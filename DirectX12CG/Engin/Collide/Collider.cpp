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
	//// 資料領域チェック①の前準備
	//Vector3D p0p1 = triangle.vertexPoint[1] - triangle.vertexPoint[0];
	//Vector3D p0p2 = triangle.vertexPoint[2] - triangle.vertexPoint[0];
	//Vector3D p0pt = point - triangle.vertexPoint[0];
	////資料領域チェック①
	//float d1 = p0p1.GetV3Dot(p0pt);
	//float d2 = p0p2.GetV3Dot(p0pt);

	//if (d1 <= 0.0f && d2 <= 0.0f)
	//{
	//	// p0が最近傍
	//	closest = triangle.vertexPoint[0];
	//	return;
	//}
	////-------------
	//// 資料領域チェック②の前準備
	//Vector3D p1pt = point - triangle.vertexPoint[1];

	//float d3 = XMVector3Dot(p0p1, p1pt);
	//float d4 = XMVector3Dot(p0p2, p1pt);

	//if (d3.m128_f32[0] >= 0.0f && d4.m128_f32[0] <= d3.m128_f32[0])
	//{
	//	// p1が最近傍
	//	*closest = triangle.p1;
	//	return;
	//}

	//// pointがp0_p1の辺領域の中にあるかどうかチェックし、あればpointのp0_p1上に対する射影を返す
	//float vc = d1.m128_f32[0] * d4.m128_f32[0] - d3.m128_f32[0] * d2.m128_f32[0];
	//if (vc <= 0.0f && d1.m128_f32[0] >= 0.0f && d3.m128_f32[0] <= 0.0f)
	//{
	//	float v = d1.m128_f32[0] / (d1.m128_f32[0] - d3.m128_f32[0]);
	//	*closest = triangle.p0 + v * p0_p1;
	//	return;
	//}

	//// pointがp2の外側の頂点領域の中にあるかどうかチェック
	//XMVECTOR p2_pt = point - triangle.p2;

	//XMVECTOR d5 = XMVector3Dot(p0_p1, p2_pt);
	//XMVECTOR d6 = XMVector3Dot(p0_p2, p2_pt);
	//if (d6.m128_f32[0] >= 0.0f && d5.m128_f32[0] <= d6.m128_f32[0])
	//{
	//	*closest = triangle.p2;
	//	return;
	//}

	//// pointがp0_p2の辺領域の中にあるかどうかチェックし、あればpointのp0_p2上に対する射影を返す
	//float vb = d5.m128_f32[0] * d2.m128_f32[0] - d1.m128_f32[0] * d6.m128_f32[0];
	//if (vb <= 0.0f && d2.m128_f32[0] >= 0.0f && d6.m128_f32[0] <= 0.0f)
	//{
	//	float w = d2.m128_f32[0] / (d2.m128_f32[0] - d6.m128_f32[0]);
	//	*closest = triangle.p0 + w * p0_p2;
	//	return;
	//}

	//// pointがp1_p2の辺領域の中にあるかどうかチェックし、あればpointのp1_p2上に対する射影を返す
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
	//衝突地点がレイの射程を超えているかレイの逆方向だった場合
	if (tempPos > ray.range || tempPos < 0)
	{
		return false;
	}
	Vector3D aVec;

	aVec.V3Get(sphere.centerPosition.vec, Temp);

	float a = aVec.V3Len();

	return a < sphere.radius + ray.radius;
}

