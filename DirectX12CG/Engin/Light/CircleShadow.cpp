#include "CircleShadow.h"
using namespace MCB;
void MCB::CircleShadow::SetDir(const Vector3D& dir)
{
	dir_ = dir;
}

const Vector3D& MCB::CircleShadow::GetDir()
{
	return dir_;
}

void MCB::CircleShadow::SetCasterPos(const Float3& casterPos)
{
	casterPos_ = casterPos;
}

const Float3& MCB::CircleShadow::GetCasterPos()
{
	return casterPos_;
}

void MCB::CircleShadow::SetDistanceCasterLight(float dist)
{
	distanceCasterLight_ = dist;
}

const float MCB::CircleShadow::GetDistCasterL()
{
	return distanceCasterLight_;
}

void MCB::CircleShadow::SetAtten(const Float3& atten)
{
	atten_ = atten;
}

const Float3& MCB::CircleShadow::GetAtten()
{
	return atten_;
}

void MCB::CircleShadow::SetFactorAngleCos(const Float2& factorAngle)
{
	factorAngleCos_ = factorAngle;
}

const Float2& MCB::CircleShadow::GetFacterAngleCos()
{
	return factorAngleCos_;
}

void MCB::CircleShadow::SetActive(bool active)
{
	active_ = active;
}

const bool MCB::CircleShadow::IsActive()
{
	return active_;
}
