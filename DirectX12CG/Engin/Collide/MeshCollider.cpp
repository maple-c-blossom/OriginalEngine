#include "MeshCollider.h"
#include "CollisionManager.h"
using namespace DirectX;
MCB::MeshCollider::MeshCollider()
{
		primitive = PrimitiveType::MESH;
}
void MCB::MeshCollider::ConstractTriangle(Model* model)
{
	triangles.clear();
	int start = 0;
	size_t triangleNum = model->indices.size() / 3;
	triangles.resize(triangles.size() + triangleNum);
	for (int i = 0; i < triangleNum; i++)
	{
		Triangle& tri = triangles[start + i];
		int idx0 = model->indices[i * 3 + 0];
		int idx1 = model->indices[i * 3 + 1];
		int idx2 = model->indices[i * 3 + 2];

		tri.vertexPoint[0].vec = { model->vertices[idx0].pos.x,model->vertices[idx0].pos.y,model->vertices[idx0].pos.z };
		tri.vertexPoint[1].vec = { model->vertices[idx1].pos.x,model->vertices[idx1].pos.y,model->vertices[idx1].pos.z };
		tri.vertexPoint[2].vec = { model->vertices[idx2].pos.x,model->vertices[idx2].pos.y,model->vertices[idx2].pos.z };
		tri.NormalCalculation();

	}
}

void MCB::MeshCollider::ConstractTriangle(AnimationModel* model)
{
	triangles.clear();
	int start = 0;

	for (int i = 0; i < model->nodes.size(); i++)
	{
		for (int j = 0; j < model->nodes[i]->meshes.size(); j++)
		{
			size_t triangleNum = model->nodes[i]->meshes[j]->indices.size() / 3;
			triangles.resize(triangles.size() + triangleNum);
			for (int i = 0; i < triangleNum; i++)
			{
				Triangle& tri = triangles[start + i];
				int idx0 = model->nodes[i]->meshes[j]->indices[i * 3 + 0];
				int idx1 = model->nodes[i]->meshes[j]->indices[i * 3 + 1];
				int idx2 = model->nodes[i]->meshes[j]->indices[i * 3 + 2];

				tri.vertexPoint[0].vec = { model->nodes[i]->meshes[j]->vertices[idx0].pos.x,model->nodes[i]->meshes[j]->vertices[idx0].pos.y,model->nodes[i]->meshes[j]->vertices[idx0].pos.z };
				tri.vertexPoint[1].vec = { model->nodes[i]->meshes[j]->vertices[idx1].pos.x,model->nodes[i]->meshes[j]->vertices[idx1].pos.y,model->nodes[i]->meshes[j]->vertices[idx1].pos.z };
				tri.vertexPoint[2].vec = { model->nodes[i]->meshes[j]->vertices[idx2].pos.x,model->nodes[i]->meshes[j]->vertices[idx2].pos.y,model->nodes[i]->meshes[j]->vertices[idx2].pos.z };
				tri.NormalCalculation();

			}
			start += (int)triangleNum;
		}
	}
	
}

void MCB::MeshCollider::Update()
{
	XMMATRIX mat = GetObject3D()->GetMatWorld();
	invWorldMat = DirectX::XMMatrixInverse(nullptr,mat);
	GetObject3D()->hited = false;
	if (sphere.GetObject3D() == nullptr)
	{
		sphere.SetObject(GetObject3D());
		sphere.SetRadius(1.5f);
		CollisionManager::GetInstance()->AddCollider(&sphere);
	}
	sphere.Update();

}

bool MCB::MeshCollider::ChakeCollisionSphere(const Sphere& sphere, Vector3D* inter)
{
	Sphere local;
	local.centerPosition = XMVector3Transform(XMVECTOR{ sphere.centerPosition.vec.x, sphere.centerPosition.vec.y, sphere.centerPosition.vec.z }, invWorldMat);
	local.radius *= XMVector3Length(invWorldMat.r[0]).m128_f32[0];

	std::vector<Triangle>::const_iterator itr = triangles.cbegin();
	for (; itr != triangles.cend(); ++itr)
	{
		const Triangle& tri = *itr;
		if (Collision::CalcTriangleSpher(tri, sphere, inter))
		{
			if (inter)
			{
				const XMMATRIX& temp = object3d->GetMatWorld();
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
	local.StartPosition = XMVector3Transform(XMVECTOR{ ray.StartPosition.vec.x,ray.StartPosition.vec.y, ray.StartPosition.vec.z }, invWorldMat);
	local.rayVec = XMVector3TransformNormal(XMVECTOR{ ray.rayVec.vec.x,ray.rayVec.vec.y, ray.rayVec.vec.z },invWorldMat);

	std::vector<Triangle>::const_iterator itr = triangles.cbegin();
	for (; itr != triangles.cend(); ++itr)
	{
		const Triangle& tri = *itr;
		Vector3D tempinter;
		if (Collision::CalcTriangleRay(tri, ray, nullptr,inter))
		{
			const XMMATRIX& temp = object3d->GetMatWorld();
			tempinter = XMVector3Transform(tempinter.ConvertXMVEC(), temp);
			if (dist)
			{
				Vector3D sub(ray.StartPosition, tempinter);
				*dist = sub.GetV3Dot(ray.rayVec);
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
