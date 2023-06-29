#include "MeshCollider.h"
#include "CollisionManager.h"
using namespace DirectX;
MCB::MeshCollider::MeshCollider()
{
	primitive_ = PrimitiveType::MESH;
}
MCB::MeshCollider::MeshCollider(Model* model)
{
	primitive_ = PrimitiveType::MESH;
	ConstractTriangle(model);
}

MCB::MeshCollider::MeshCollider(AnimationModel* model)
{
	primitive_ = PrimitiveType::MESH;
	ConstractTriangle(model);
}
void MCB::MeshCollider::ConstractTriangle(Model* model)
{
	triangles_.clear();
	int32_t start = 0;
	size_t triangleNum = model->indices_.size() / 3;
	triangles_.resize(triangles_.size() + triangleNum);
	for (int32_t i = 0; i < triangleNum; i++)
	{
		Triangle& tri = triangles_[start + i];
		int32_t idx0 = model->indices_[i * 3 + 0];
		int32_t idx1 = model->indices_[i * 3 + 1];
		int32_t idx2 = model->indices_[i * 3 + 2];

		tri.vertexPoint_[0].vec_ = { model->vertices_[idx0].pos.x_,
			model->vertices_[idx0].pos.y_,model->vertices_[idx0].pos.z_ };

		tri.vertexPoint_[1].vec_ = { model->vertices_[idx1].pos.x_,
			model->vertices_[idx1].pos.y_,model->vertices_[idx1].pos.z_ };

		tri.vertexPoint_[2].vec_ = { model->vertices_[idx2].pos.x_,
			model->vertices_[idx2].pos.y_,model->vertices_[idx2].pos.z_ };
		tri.NormalCalculation();
	}
}

void MCB::MeshCollider::ConstractTriangle(AnimationModel* model)
{
	triangles_.clear();
	int32_t start = 0;

	for (int32_t i = 0; i < model->nodes_.size(); i++)
	{
		for (int32_t j = 0; j < model->nodes_[i]->meshes.size(); j++)
		{
			size_t triangleNum = model->nodes_[i]->meshes[j]->indices_.size() / 3;
			triangles_.resize(triangles_.size() + triangleNum);
			for (int32_t i = 0; i < triangleNum; i++)
			{
				Triangle& tri = triangles_[start + i];
				int32_t idx0 = model->nodes_[i]->meshes[j]->indices_[i * 3 + 0];
				int32_t idx1 = model->nodes_[i]->meshes[j]->indices_[i * 3 + 1];
				int32_t idx2 = model->nodes_[i]->meshes[j]->indices_[i * 3 + 2];

				tri.vertexPoint_[0].vec_ = { model->nodes_[i]->meshes[j]->vertices_[idx0].pos.x_,
					model->nodes_[i]->meshes[j]->vertices_[idx0].pos.y_
					,model->nodes_[i]->meshes[j]->vertices_[idx0].pos.z_ };

				tri.vertexPoint_[1].vec_ = { model->nodes_[i]->meshes[j]->vertices_[idx1].pos.x_,
					model->nodes_[i]->meshes[j]->vertices_[idx1].pos.y_,
					model->nodes_[i]->meshes[j]->vertices_[idx1].pos.z_ };

				tri.vertexPoint_[2].vec_ = { model->nodes_[i]->meshes[j]->vertices_[idx2].pos.x_,
					model->nodes_[i]->meshes[j]->vertices_[idx2].pos.y_,
					model->nodes_[i]->meshes[j]->vertices_[idx2].pos.z_ };

				tri.NormalCalculation();

			}
			start += (int32_t)triangleNum;
		}
	}
	
}

void MCB::MeshCollider::Update()
{
	XMMATRIX mat = GetObject3D()->GetMatWorld();
	invWorldMat_ = DirectX::XMMatrixInverse(nullptr,mat);
	GetObject3D()->hited_ = false;
	//if (sphere.GetObject3D() == nullptr)
	//{
	//	sphere.SetObject(GetObject3D());
	//	sphere.SetRadius(1.5f);
	//	CollisionManager::GetInstance()->AddCollider(&sphere);
	//}
	//sphere.Update();

}

bool MCB::MeshCollider::ChakeCollisionSphere(const Sphere& sphere, Vector3D* inter, Vector3D* reject)
{
	Sphere local;
	local.centerPosition_ = XMVector3Transform(XMVECTOR{ sphere.centerPosition_.vec_.x_, sphere.centerPosition_.vec_.y_, sphere.centerPosition_.vec_.z_ }, invWorldMat_);
	local.radius_ = sphere.radius_ * XMVector3Length(invWorldMat_.r[0]).m128_f32[0];

	std::vector<Triangle>::const_iterator itr = triangles_.cbegin();
	for (; itr != triangles_.cend(); ++itr)
	{
		const Triangle& tri = *itr;
		if (Collision::CalcTriangleSpher(tri, local, inter,reject))
		{
			const XMMATRIX& temp = object3d_->GetMatWorld();
			if (reject)
			{
				*reject = XMVector3TransformNormal(reject->ConvertXMVEC(), temp);
			}

			if (inter)
			{
	
				*inter = XMVector3Transform(inter->ConvertXMVEC(), temp);
			}
			return true;
		}
	}
	return false;
}

bool MCB::MeshCollider::ChakeCollisionRay(const Ray& ray, float* dist, Vector3D* inter)
{
	Ray local;
	local.StartPosition_ = XMVector3Transform(XMVECTOR{ ray.StartPosition_.vec_.x_,ray.StartPosition_.vec_.y_, ray.StartPosition_.vec_.z_,1 }, invWorldMat_);
	local.rayVec_ = XMVector3TransformNormal(
		XMVECTOR{ ray.rayVec_.vec_.x_,ray.rayVec_.vec_.y_, ray.rayVec_.vec_.z_,1 },invWorldMat_);

	std::vector<Triangle>::const_iterator itr = triangles_.cbegin();
	for (; itr != triangles_.cend(); ++itr)
	{
		const Triangle& tri = *itr;
		Vector3D tempinter;
		if (Collision::CalcTriangleRay(tri, local, nullptr, &tempinter))
		{
			const XMMATRIX& temp = object3d_->GetMatWorld();
			tempinter = XMVector3Transform(tempinter.ConvertXMVEC(), temp);
			if (dist)
			{
				Vector3D tempV = ray.StartPosition_;
				Vector3D sub = tempinter - tempV;
				*dist = sub.GetV3Dot(ray.rayVec_);
			}
			if (inter)
			{
				*inter = tempinter;
			}
			return true;
		}
	}
	return false;
}
