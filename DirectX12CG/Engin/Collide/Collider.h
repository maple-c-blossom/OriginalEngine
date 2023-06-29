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

	const uint16_t ATTRIBUTE_LANDSHAPE = 0b1 << 0;//’n–Ê
	const uint16_t ATTRIBUTE_FLENDRY = 0b1 << 1;//–¡•û
	const uint16_t ATTRIBUTE_ENEMY = 0b1 << 2;//“G

	class Triangle
	{
	public:
		std::array<Vector3D,3> vertexPoint_ = {};
		Vector3D normal_ = {};
		void NormalCalculation();
	};
	class Ray
	{
	public:
		Vector3D StartPosition_ = {};
		Vector3D rayVec_ = {};
		float range_ = 0;
		float radius_ = 0;
		bool rayCasted_ = false;
	};

	class Box
	{
	public:
		Float3 position_ = {};
		Float3 size_ = {};
	};

	class Sphere
	{
	public:
		Vector3D centerPosition_ = {};
		float radius_ = 1.0f;
	};

	class Plane
	{
	public:
		Vector3D normal_ = {0,1,0};
		float originDistance_ = 0;//Œ´“_‚©‚ç‚Ì‹——£
	};


	class Collision
	{

	public:
		static bool CalcRaySphere(const Ray& ray,const Sphere& sphere, float* distance = nullptr, Vector3D* inter = nullptr);
		static bool CalcRaySphere( Ray ray,const Sphere& sphere);
		static bool CalcSphere(const Sphere& sphereA,const Sphere& sphereB, Vector3D* inter = nullptr,Vector3D* reject = nullptr);
		static bool CalcPlaneRay( Plane plane,const Ray& ray, float* distance = nullptr,Vector3D* inter = nullptr);
		static bool CalcTriangleRay( Triangle triangle,const Ray& ray, float* distance = nullptr,Vector3D* inter = nullptr);
		static bool CalcPlaneSpher(const Plane& plane, Sphere sphere,Vector3D* inter = nullptr);
		static bool CalcTriangleSpher( Triangle triangle, Sphere sphere, Vector3D* inter = nullptr, Vector3D* reject = nullptr);
		static void CalcTrianglePoint(const Triangle& triangle, const Vector3D& sphere, Vector3D& point);
	};
}

