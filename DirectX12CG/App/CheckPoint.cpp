#include "CheckPoint.h"

using namespace std;
using namespace MCB;




void CheckPoint::Init()
{
	Object3d::Init();
	SetCollider(make_unique<SphereCollider>(Vector3D{ 0,0,0 }, 2.f));
	collider_->SetAttribute(ATTRIBUTE_ENEMY);
	UpdateMatrix();
	nameId_ = "CheckPoint";
	collider_->Update();
}
