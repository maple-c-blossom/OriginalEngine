#include "Collider.h"
#include "Util.h"
using namespace MCB;

bool MCB::Collision::CalcSphere(const Sphere& sphereA, const Sphere& sphereB, Vector3D* inter,Vector3D* reject)
{

	float hitX = (sphereA.centerPosition_.vec_.x_ - sphereB.centerPosition_.vec_.x_) * (sphereA.centerPosition_.vec_.x_ - sphereB.centerPosition_.vec_.x_);
	float hitY = (sphereA.centerPosition_.vec_.y_ - sphereB.centerPosition_.vec_.y_) * (sphereA.centerPosition_.vec_.y_ - sphereB.centerPosition_.vec_.y_);
	float hitZ = (sphereA.centerPosition_.vec_.z_ - sphereB.centerPosition_.vec_.z_) * (sphereA.centerPosition_.vec_.z_ - sphereB.centerPosition_.vec_.z_);
	float hitR = (sphereB.radius_ + sphereA.radius_) * (sphereB.radius_ + sphereA.radius_);
	hitX = Abs(hitX);
	hitY = Abs(hitY);
	hitZ = Abs(hitZ);
	float hit = hitX + hitY + hitZ;


	if (hit <= hitR)
	{
		if (reject)
		{
			float reLen = sphereA.radius_ + sphereB.radius_ - sqrt(hit);
			Vector3D hoge(sphereB.centerPosition_, sphereA.centerPosition_);
			hoge.V3Norm();
			*reject = hoge;
			*reject *= reLen;
		}

		if (inter)
		{
			float t = sphereB.radius_ / (sphereA.radius_ + sphereB.radius_);
			inter->vec_.x_ = (float)Lerp(sphereA.centerPosition_.vec_.x_, sphereB.centerPosition_.vec_.x_, t);
			inter->vec_.y_ = (float)Lerp(sphereA.centerPosition_.vec_.y_, sphereB.centerPosition_.vec_.y_, t);
			inter->vec_.z_ = (float)Lerp(sphereA.centerPosition_.vec_.z_, sphereB.centerPosition_.vec_.z_, t);
		}
		return true;
	}
	return false;
}

bool MCB::Collision::CalcPlaneRay(Plane plane, const Ray& ray, float* distance, Vector3D* inter)
{
	const float epsilon = 1.0e-5f;
	float d1 = plane.normal_.GetV3Dot(ray.rayVec_);
	if (d1 > -epsilon) { return false; }
	float d2 = plane.normal_.GetV3Dot(ray.StartPosition_);
	float dist = d2 - plane.originDistance_;
	float t = dist / -d1;
	if (t < 0) { return false; }
	if (distance) { *distance = t; }
	if (inter) 
	{ 
		Vector3D temp = ray.rayVec_ * t;
		*inter = ray.StartPosition_ + temp;
	}
	return true;
}

bool MCB::Collision::CalcTriangleRay(Triangle triangle,const Ray& ray, float* distance, Vector3D* inter)
{
	Plane plane;
	Vector3D interPlane;
	plane.normal_ = triangle.normal_;
	plane.originDistance_ = triangle.normal_.GetV3Dot(triangle.vertexPoint_[0]);
	if (!CalcPlaneRay(plane, ray, distance, &interPlane)) { return false; }
	const float epsilon = 1.0e-5f;
	Vector3D m;
	Vector3D ptp0(interPlane, triangle.vertexPoint_[0]);
	Vector3D p0p1(triangle.vertexPoint_[0], triangle.vertexPoint_[1]);
	m = ptp0.GetV3Cross(p0p1);
	if (m.GetV3Dot(triangle.normal_) < -epsilon) { return false; }

	Vector3D ptp1(interPlane, triangle.vertexPoint_[1]);
	Vector3D p1p2(triangle.vertexPoint_[1], triangle.vertexPoint_[2]);
	m = ptp1.GetV3Cross(p1p2);
	if (m.GetV3Dot(triangle.normal_) < -epsilon) { return false; }

	Vector3D ptp2(interPlane, triangle.vertexPoint_[2]);
	Vector3D p2p0(triangle.vertexPoint_[2], triangle.vertexPoint_[0]);
	m = ptp2.GetV3Cross(p2p0);
	if (m.GetV3Dot(triangle.normal_) < -epsilon) { return false; }

	if (inter) { *inter = interPlane; };

	return true;
}

bool MCB::Collision::CalcPlaneSpher(const Plane& plane, Sphere sphere, Vector3D* inter)
{
	float dist = sphere.centerPosition_.GetV3Dot(plane.normal_);
	dist = dist - plane.originDistance_;
	if (fabsf(dist) > sphere.radius_) return false;
	if (inter) *inter = -dist * plane.normal_ + sphere.centerPosition_;
	return true;
}

bool MCB::Collision::CalcTriangleSpher(Triangle triangle, Sphere sphere, Vector3D* inter,Vector3D* reject)
{
	Vector3D p;
	CalcTrianglePoint(triangle, sphere.centerPosition_, p);
	Vector3D v(sphere.centerPosition_, p);
	float vDot = v.GetV3Dot(v);
	if (vDot > sphere.radius_ * sphere.radius_) return false;
	if (reject)
	{
		float ds = sphere.centerPosition_.GetV3Dot(triangle.normal_);
		float dt = triangle.vertexPoint_[0].GetV3Dot(triangle.normal_);
		float rejectLen = dt - ds + sphere.radius_;
		*reject = triangle.normal_ * rejectLen;
	}
	if (inter) *inter = p;
	return true;
}

void MCB::Collision::CalcTrianglePoint(const Triangle& triangle,const Vector3D& point, Vector3D& closest)
{
	// 資料領域チェック①の前準備
	Vector3D p0p1 = triangle.vertexPoint_[1] - triangle.vertexPoint_[0];
	Vector3D p0p2 = triangle.vertexPoint_[2] - triangle.vertexPoint_[0];
	Vector3D p0pt = point - triangle.vertexPoint_[0];
	//資料領域チェック①
	float d1 = p0p1.GetV3Dot(p0pt);
	float d2 = p0p2.GetV3Dot(p0pt);

	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		// p0が最近傍
		closest = triangle.vertexPoint_[0];
		return;
	}
	//-------------
	// 資料領域チェック②の前準備
	Vector3D p1pt = point - triangle.vertexPoint_[1];

	float d3 = p0p1.GetV3Dot(p1pt);
	float d4 = p0p2.GetV3Dot(p1pt);

	if (d3 >= 0.0f && d4 <= d3)
	{
		// p1が最近傍
		closest = triangle.vertexPoint_[1];
		return;
	}

	// pointがp0_p1の辺領域の中にあるかどうかチェックし、あればpointのp0_p1上に対する射影を返す
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		closest = triangle.vertexPoint_[0];
		Vector3D temp = p0p1 * v;
		closest += temp;
		return;
	}

	// pointがp2の外側の頂点領域の中にあるかどうかチェック
	Vector3D p2pt = point - triangle.vertexPoint_[2];

	float d5 = p0p1.GetV3Dot(p2pt);
	float d6 = p0p2.GetV3Dot(p2pt);
	if (d6 >= 0.0f && d5 <= d6)
	{
		closest = triangle.vertexPoint_[2];
		return;
	}

	// pointがp0_p2の辺領域の中にあるかどうかチェックし、あればpointのp0_p2上に対する射影を返す
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		closest = triangle.vertexPoint_[0] + w * p0p2;
		return;
	}

	// pointがp1_p2の辺領域の中にあるかどうかチェックし、あればpointのp1_p2上に対する射影を返す
	float va = d3 * d6 - d5* d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		closest = triangle.vertexPoint_[1] + w * (triangle.vertexPoint_[2] - triangle.vertexPoint_[1]);
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	closest = triangle.vertexPoint_[0] + p0p1 * v + p0p2 * w;

}

bool MCB::Collision::CalcRaySphere(const Ray& ray,const Sphere& sphere, float* distance, Vector3D* inter)
{
	Vector3D m(sphere.centerPosition_, ray.StartPosition_);
	float b = m.GetV3Dot(ray.rayVec_);
	float c = m.GetV3Dot(m) - sphere.radius_ * sphere.radius_;
	if (c > 0.f && b > 0.f) return false;
	float discr = b * b - c;
	if (discr < 0.f) return false;
	float t = -b - sqrt(discr);
	if (t < 0) t = 0.0f;
	if (distance) *distance = t;
	if (inter) 
	{
		Vector3D temp = ray.rayVec_ * t;
		*inter = ray.StartPosition_ + temp;
	}
		return true;
}

bool MCB::Collision::CalcRaySphere( Ray ray,const Sphere& sphere)
{
	Vector3D rayToSphere(ray.StartPosition_, sphere.centerPosition_.vec_);
	ray.rayVec_.V3Norm();
	float tempPos = rayToSphere.GetV3Dot(ray.rayVec_);
	ray.rayVec_ = ray.rayVec_ * tempPos;
	Float3 Temp;
	Temp.x_ = ray.rayVec_.vec_.x_ + ray.StartPosition_.vec_.x_;
	Temp.y_ = ray.rayVec_.vec_.y_ + ray.StartPosition_.vec_.y_;
	Temp.z_ = ray.rayVec_.vec_.z_ + ray.StartPosition_.vec_.z_;
	//衝突地点がレイの射程を超えているかレイの逆方向だった場合
	if (tempPos > ray.range_ || tempPos < 0)
	{
		return false;
	}
	Vector3D aVec;

	aVec.V3Get(sphere.centerPosition_.vec_, Temp);

	float a = aVec.V3Len();

	return a < sphere.radius_ + ray.radius_;
}



void MCB::Triangle::NormalCalculation()
{
	Vector3D p0p1(vertexPoint_[0], vertexPoint_[1]);
	Vector3D p0p2(vertexPoint_[0], vertexPoint_[2]);

	normal_ = p0p1.GetV3Cross(p0p2);
	normal_.V3Norm();

}
