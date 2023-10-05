#include "PointLight.h"
using namespace MCB;
void MCB::PointLight::SetPLightPos(const Float3& pos)
{
	lightPos_ = pos;
}

void MCB::PointLight::SetPLightColor(const Float3& color)
{
	lightColor_ = color;
}

void MCB::PointLight::SetPLightAtten(const Float3& atten)
{
	lightAtten_ = atten;
}

void MCB::PointLight::SetPLightShininess(float shininess)
{
	shininess_ = shininess;
}

void MCB::PointLight::SetIsActive(bool flag)
{
	isActive_ = flag;
}

Float3 MCB::PointLight::GetPLightPos()
{
	return lightPos_;
}

Float3 MCB::PointLight::GetPLightColor()
{
	return lightColor_;
}

Float3 MCB::PointLight::GetPLightAtten()
{
	return lightAtten_;
}

float MCB::PointLight::GetPLightShininess()
{
	return shininess_;
}

bool MCB::PointLight::IsActive()
{
	return isActive_;
}
