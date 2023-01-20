#pragma once
#include <cmath>
#include "Vector3D.h"
#include <array>

namespace MCB
{
	class Triangle
	{
		std::array<Vector3D,3> vertexPoint;
		Vector3D normal;

	};
	class Ray
	{
	public:
		Float3 StartPosition;
		Vector3D rayVec;
		float range;
		float radius;
	};

	class Box
	{
	public:
		Float3 position;
		Float3 size;
	};

	class Sphere
	{
	public:
		Vector3D centerPosition;
		float radius;
	};

	class Plane
	{
	public:
		Vector3D normal = {0,1,0};
		float distance = 0;
	};

	bool CalcRaySphere(Ray ray,Sphere sphere);
	bool CalcSphere(Sphere sphereA,Sphere sphereB);
	bool CalcPlaneSpher(Plane plane,Sphere sphere);
	bool CalcTriangleSpher(Triangle plane,Sphere sphere);
}

