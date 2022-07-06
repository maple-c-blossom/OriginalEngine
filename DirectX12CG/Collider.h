#pragma once
#include <cmath>
#include "Vector3D.h"

namespace MCB
{

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
		Float3 position;
		float radius;
	};

	bool CalcRaySphere(Ray ray,Sphere sphere);
	bool CalcSphere(Sphere sphereA,Sphere sphereB);
}

