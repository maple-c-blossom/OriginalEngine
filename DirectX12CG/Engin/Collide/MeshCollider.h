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
		MeshCollider(Model* model);
		MeshCollider(AnimationModel* model);
		void ConstractTriangle(Model* model);
		void ConstractTriangle(AnimationModel* model);
		void Update()override;
		bool ChakeCollisionSphere(const Sphere& sphere, Vector3D* inter = nullptr,Vector3D* reject = nullptr);
		bool ChakeCollisionRay(const Ray& ray,float* dist ,Vector3D* inter = nullptr);

		//SphereCollider sphere;
	private:
		std::vector<Triangle>triangles_;
		DirectX::XMMATRIX invWorldMat_;
	};
}
