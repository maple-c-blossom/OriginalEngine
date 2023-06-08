#pragma once
#include <cmath>
#include "Vector3D.h"
#include <array>

namespace MCB
{
	enum class PrimitiveType
	{
		SPHERE,
		RAY,
		TRIANGLE,
		BOX,
		PLANE,
		MESH
	};

	const uint16_t attributeLANDSHAPE = 0b1 << 0;//’n–Ê
	const uint16_t attributeFLENDRY = 0b1 << 1;//–¡•û
	const uint16_t attributeENEMY = 0b1 << 2;//“G

	class Triangle
	{
	public:
		std::array<Vector3D,3> vertexPoint = {};
		Vector3D normal = {};
		void NormalCalculation();
	};
	class Ray
	{
	public:
		Vector3D StartPosition = {};
		Vector3D rayVec = {};
		float range = 0;
		float radius = 0;
		bool rayCasted = false;
	};

	class Box
	{
	public:
		Float3 position = {};
		Float3 size = {};
	};

	class Sphere
	{
	public:
		Vector3D centerPosition = {};
		float radius = {};
	};

	class Plane
	{
	public:
		Vector3D normal = {0,1,0};
		float originDistance = 0;//Œ´“_‚©‚ç‚Ì‹——£
	};


	class Collision
	{

	public:
		static bool CalcRaySphere(const Ray& ray,const Sphere& sphere, float* distance = nullptr, Vector3D* inter = nullptr);
		static bool CalcRaySphere(const Ray& ray,const Sphere& sphere);
		static bool CalcSphere(const Sphere& sphereA,const Sphere& sphereB, Vector3D* inter = nullptr,Vector3D* reject = nullptr);
		static bool CalcPlaneRay(const Plane& plane,const Ray& ray, float* distance = nullptr,Vector3D* inter = nullptr);
		static bool CalcTriangleRay(const Triangle& triangle,const Ray& ray, float* distance = nullptr,Vector3D* inter = nullptr);
		static bool CalcPlaneSpher(const Plane& plane,const Sphere& sphere,Vector3D* inter = nullptr);
		static bool CalcTriangleSpher(const Triangle& triangle,const Sphere& sphere, Vector3D* inter = nullptr, Vector3D* reject = nullptr);
		static void CalcTrianglePoint(const Triangle triangle, Vector3D sphere, Vector3D& point);
	};
}

