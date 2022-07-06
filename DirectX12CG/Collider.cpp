#include "Collider.h"
#include "Util.h"
using namespace MCB;

bool MCB::CalcSphere(Sphere sphereA, Sphere sphereB)
{

	int hitX = (sphereA.position.x - sphereB.position.x) * (sphereA.position.x - sphereB.position.x);
	int hitY = (sphereA.position.y - sphereB.position.y) * (sphereA.position.y - sphereB.position.y);
	int hitZ = (sphereA.position.z - sphereB.position.z) * (sphereA.position.z - sphereB.position.z);
	int hitR = (sphereB.radius + sphereA.radius) * (sphereB.radius + sphereA.radius);
	hitX = Abs(hitX);
	hitY = Abs(hitY);
	hitZ = Abs(hitZ);
	int hit = hitX + hitY + hitZ;
	if (hit <= hitR)
	{
		return true;
	}
	return false;
}

bool MCB::CalcRaySphere(Ray ray, Sphere sphere)
{
	Vector3D rayToSphere(ray.StartPosition, sphere.position);
	ray.rayVec.V3Norm();
	float tempPos = rayToSphere.GetV3Dot(ray.rayVec);
	ray.rayVec = ray.rayVec * tempPos;

	//衝突地点がレイの射程を超えているかレイの逆方向だった場合
	if (tempPos > ray.range || tempPos < 0)
	{
		return false;
	}

	Float3 rayHitPosition;
	(Vector3D)rayHitPosition = (ray.rayVec * tempPos);
	(Vector3D)rayHitPosition += (Vector3D)ray.StartPosition;


	Sphere Sray;
	Sray.radius = ray.radius;
	Sray.position = rayHitPosition;

	return CalcSphere(Sray,sphere);
}


