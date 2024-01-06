#pragma once
#include "Object3d.h"
#include "SphereCollider.h"
#include "PopEffect.h"
#include "IgnoreWarning.h"
WarningIgnoreBegin
#include <vector>
WarningIgnoreEnd
class JumpPad : public MCB::Object3d
{
public:
	MCB::Vector3D jumpVec = { 0,-1,0 };

};

