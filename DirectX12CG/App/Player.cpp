#include "Player.h"
#include "Input.h"
#include "CollisionManager.h"
void MCB::Player::Init()
{
	Object3d::Init();
	SetCollider(new SphereCollider);
	collider->SetAttribute(attributeFLENDRY);
}

void MCB::Player::Update()
{


	if (Input::GetInstance()->IsKeyDown(DIK_A))
	{
		position.x -= 0.05f;
	}
	if (Input::GetInstance()->IsKeyDown(DIK_D))
	{
		position.x += 0.05f;
	}
	if (Input::GetInstance()->IsKeyDown(DIK_W))
	{
		position.z += 0.1f;
	}
	if (Input::GetInstance()->IsKeyDown(DIK_S))
	{
		position.z -= 0.1f;
	}
	if (!isGraund)
	{
		const float fallAcc = -0.01f;
		const float VYMin = -0.5f;
		fallV.vec.y = max(fallV.vec.y + fallAcc, VYMin);
		position.x += fallV.vec.x;
		position.y += fallV.vec.y;
		position.z += fallV.vec.z;
	}
	else if (Input::GetInstance()->IsKeyTrigger(DIK_SPACE))
	{
		isGraund = false;
		const float jumpVYFist = 0.2f;
		fallV = { 0,jumpVYFist,0,0};
	}

	Object3d::Update();
	SphereCollider* sphere = dynamic_cast<SphereCollider*>(collider);
	assert(sphere);

	Ray ray;
	ray.StartPosition = sphere->centerPosition;
	ray.StartPosition.vec.y += sphere->GetRaius();
	ray.rayVec = { 0,-1,0,0 };
	RayCastHit info;
	if (isGraund)
	{
		const float absDistance = 0.2f;
		if (CollisionManager::GetInstance()->Raycast(ray, attributeLANDSHAPE, &info, sphere->GetRaius() * 2.0f))
		{
			isGraund = true;
			position.y -= (info.dist - sphere->GetRaius() * 2.0f);
			Object3d::Update();
		}
		else
		{
			isGraund = false;
			fallV = {0,0,0,0};
		}
	}
	else if(fallV.vec.y <= 0.0f)
	{
		if (CollisionManager::GetInstance()->Raycast(ray, attributeLANDSHAPE, &info, sphere->GetRaius() * 2.0f))
		{
			isGraund = true;
			position.y -= (info.dist - sphere->GetRaius() * 2.0f);
			Object3d::Update();
		}
	}
}




void MCB::Player::Draw()
{
	Object3d::Draw();
}
