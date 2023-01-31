#include "Collider.h"
#include "Util.h"
using namespace MCB;

bool MCB::Corider::CalcSphere(Sphere sphereA, Sphere sphereB)
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

bool MCB::Corider::CalcPlaneRay(Plane plane, Ray ray, float* distance, Vector3D* inter)
{
	const float epsilon = 1.0e-5f;
	float d1 = plane.normal.GetV3Dot(ray.rayVec);
	if (d1 > -epsilon) { return false; }
	float d2 = plane.normal.GetV3Dot(ray.StartPosition);
	float dist = d2 - plane.originDistance;
	float t = dist / -d1;
	if (t < 0) { return false; }
	if (distance) { *distance = t; }
	if (inter) 
	{ 
		Vector3D temp = ray.rayVec * t;
		*inter = ray.StartPosition + temp;
	}
	return true;
}

bool MCB::Corider::CalcTriangleRay(Triangle triangle, Ray ray, float* distance, Vector3D* inter)
{
	Plane plane;
	Vector3D interPlane;
	plane.normal = triangle.normal;
	plane.originDistance = triangle.normal.GetV3Dot(triangle.vertexPoint[0]);
	if (!CalcPlaneRay(plane, ray, distance, &interPlane)) { return false; }
	const float epsilon = 1.0e-5f;
	Vector3D m;
	Vector3D ptp0(interPlane, triangle.vertexPoint[0]);
	Vector3D p0p1(triangle.vertexPoint[0], triangle.vertexPoint[1]);
	m = ptp0.GetV3Cross(p0p1);
	if (m.GetV3Dot(triangle.normal) < -epsilon) { return false; }

	Vector3D ptp1(interPlane, triangle.vertexPoint[1]);
	Vector3D p1p2(triangle.vertexPoint[1], triangle.vertexPoint[2]);
	m = ptp1.GetV3Cross(p1p2);
	if (m.GetV3Dot(triangle.normal) < -epsilon) { return false; }

	Vector3D ptp2(interPlane, triangle.vertexPoint[2]);
	Vector3D p2p0(triangle.vertexPoint[2], triangle.vertexPoint[0]);
	m = ptp2.GetV3Cross(p2p0);
	if (m.GetV3Dot(triangle.normal) < -epsilon) { return false; }

	if (inter) { *inter = interPlane; };

	return true;
}

bool MCB::Corider::CalcPlaneSpher(Plane plane, Sphere sphere, Vector3D* inter)
{
	float dist = sphere.centerPosition.GetV3Dot(plane.normal);
	dist = dist - plane.originDistance;
	if (fabsf(dist) > sphere.radius) return false;
	if (inter) *inter = -dist * plane.normal + sphere.centerPosition;
	return true;
}

bool MCB::Corider::CalcTriangleSpher(Triangle triangle, Sphere sphere, Vector3D* inter)
{
	Vector3D p;
	CalcTrianglePoint(triangle, sphere.centerPosition, p);
	Vector3D v(sphere.centerPosition, p);
	float vDot = v.GetV3Dot(v);
	if (vDot > sphere.radius * sphere.radius) return false;
	if (inter) *inter = p;
	return true;
}

void MCB::Corider::CalcTrianglePoint(Triangle triangle, Vector3D point, Vector3D& closest)
{
	// 資料領域チェック①の前準備
	Vector3D p0p1 = triangle.vertexPoint[1] - triangle.vertexPoint[0];
	Vector3D p0p2 = triangle.vertexPoint[2] - triangle.vertexPoint[0];
	Vector3D p0pt = point - triangle.vertexPoint[0];
	//資料領域チェック①
	float d1 = p0p1.GetV3Dot(p0pt);
	float d2 = p0p2.GetV3Dot(p0pt);

	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		// p0が最近傍
		closest = triangle.vertexPoint[0];
		return;
	}
	//-------------
	// 資料領域チェック②の前準備
	Vector3D p1pt = point - triangle.vertexPoint[1];

	float d3 = p0p1.GetV3Dot(p1pt);
	float d4 = p0p2.GetV3Dot(p1pt);

	if (d3 >= 0.0f && d4 <= d3)
	{
		// p1が最近傍
		closest = triangle.vertexPoint[1];
		return;
	}

	// pointがp0_p1の辺領域の中にあるかどうかチェックし、あればpointのp0_p1上に対する射影を返す
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		closest = triangle.vertexPoint[0];
		Vector3D temp = p0p1 * v;
		closest += temp;
		return;
	}

	// pointがp2の外側の頂点領域の中にあるかどうかチェック
	Vector3D p2pt = point - triangle.vertexPoint[2];

	float d5 = p0p1.GetV3Dot(p2pt);
	float d6 = p0p2.GetV3Dot(p2pt);
	if (d6 >= 0.0f && d5 <= d6)
	{
		closest = triangle.vertexPoint[2];
		return;
	}

	// pointがp0_p2の辺領域の中にあるかどうかチェックし、あればpointのp0_p2上に対する射影を返す
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		closest = triangle.vertexPoint[0] + w * p0p2;
		return;
	}

	// pointがp1_p2の辺領域の中にあるかどうかチェックし、あればpointのp1_p2上に対する射影を返す
	float va = d3 * d6 - d5* d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		closest = triangle.vertexPoint[1] + w * (triangle.vertexPoint[2] - triangle.vertexPoint[1]);
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	closest = triangle.vertexPoint[0] + p0p1 * v + p0p2 * w;

}

bool MCB::Corider::CalcRaySphere(Ray ray, Sphere sphere, float* distance, Vector3D* inter)
{
	Vector3D m(sphere.centerPosition, ray.StartPosition);
	float b = m.GetV3Dot(ray.rayVec);
	float c = m.GetV3Dot(m) - sphere.radius * sphere.radius;
	if (c > 0.f && b > 0.f) return false;
	float discr = b * b - c;
	if (discr < 0.f) return false;
	float t = -b - sqrt(discr);
	if (t < 0) t = 0.0f;
	if (distance) *distance = t;
	if (inter) 
	{
		Vector3D temp = ray.rayVec * t;
		*inter = ray.StartPosition + temp;
	}
		return true;
}

bool MCB::Corider::CalcRaySphere(Ray ray, Sphere sphere)
{
	Vector3D rayToSphere(ray.StartPosition, sphere.centerPosition.vec);
	ray.rayVec.V3Norm();
	float tempPos = rayToSphere.GetV3Dot(ray.rayVec);
	ray.rayVec = ray.rayVec * tempPos;
	Float3 Temp;
	Temp.x = ray.rayVec.vec.x + ray.StartPosition.vec.x;
	Temp.y = ray.rayVec.vec.y + ray.StartPosition.vec.y;
	Temp.z = ray.rayVec.vec.z + ray.StartPosition.vec.z;
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

void MCB::Triangle::NormalCalculation()
{
	Vector3D p0p1(vertexPoint[0], vertexPoint[1]);
	Vector3D p0p2(vertexPoint[0], vertexPoint[2]);

	normal = p0p1.GetV3Cross(p0p2);
	normal.V3Norm();

}
