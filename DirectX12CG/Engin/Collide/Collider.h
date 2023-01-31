#pragma once
#include <cmath>
#include "Vector3D.h"
#include <array>

namespace MCB
{
	class Triangle
	{
	public:
		std::array<Vector3D,3> vertexPoint;
		Vector3D normal;
		void NormalCalculation();
	};
	class Ray
	{
	public:
		Vector3D StartPosition;
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
		float originDistance = 0;//Œ´“_‚©‚ç‚Ì‹——£
	};

	class Corider
	{

	public:
		static bool CalcRaySphere(Ray ray,Sphere sphere, float* distance = nullptr, Vector3D* inter = nullptr);
		static bool CalcRaySphere(Ray ray,Sphere sphere);
		static bool CalcSphere(Sphere sphereA,Sphere sphereB);
		static bool CalcPlaneRay(Plane plane,Ray ray, float* distance = nullptr,Vector3D* inter = nullptr);
		static bool CalcTriangleRay(Triangle triangle,Ray ray, float* distance = nullptr,Vector3D* inter = nullptr);
		static bool CalcPlaneSpher(Plane plane,Sphere sphere,Vector3D* inter = nullptr);
		static bool CalcTriangleSpher(Triangle triangle,Sphere sphere, Vector3D* inter = nullptr);
		static void CalcTrianglePoint(Triangle triangle, Vector3D sphere, Vector3D& point);
	};
}

