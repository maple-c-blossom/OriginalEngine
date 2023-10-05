#include "Light.h"
#include <memory>
using namespace MCB;

void MCB::DirLight::SetLightDir(const Vector3D& lightdir)
{
	lightdir_ = lightdir;
}

Vector3D MCB::DirLight::GetLightDir()
{
	return lightdir_;
}

void MCB::DirLight::SetLightColor(const Float3& lightcolor)
{
	lightColor_ = lightcolor;
}

Float3 MCB::DirLight::GetLightColor()
{
	return lightColor_;
}

void MCB::DirLight::SetLightShininess(float lightshininess)
{
	shininess_ = lightshininess;
}

float MCB::DirLight::GetLightShininess()
{
	return shininess_;
}

void MCB::DirLight::SetIsActive(bool active)
{
	active_ = active;
}

bool MCB::DirLight::IsActive()
{
	return active_;
}