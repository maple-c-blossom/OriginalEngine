#pragma once
#include "BaseCollider.h"
#include "Collider.h"
#include "SphereCollider.h"
namespace MCB
{
	class MeshCollider:public BaseCollider
	{
	public:
		MeshCollider();
		void ConstractTriangle(Model* model);
		void ConstractTriangle(AnimationModel* model);
		void Update()override;
		bool ChakeCollisionSphere(const Sphere& sphere, Vector3D* inter);
		bool ChakeCollisionRay(const Ray& ray,float* dist ,Vector3D* inter);

		SphereCollider sphere;
	private:
		std::vector<Triangle>triangles;
		DirectX::XMMATRIX invWorldMat;
	};
}
