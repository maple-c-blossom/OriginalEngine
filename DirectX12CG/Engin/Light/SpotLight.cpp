#include "SpotLight.h"
using namespace MCB;
void MCB::SpotLight::SetSLightDir(const Vector3D& lightDir)
{
	ligntDir_ = lightDir;
}

void MCB::SpotLight::SetSLightColor(const Float3& lightcolor)
{
	lightColor_ = lightcolor;
}

void MCB::SpotLight::SetSLightPos(const Float3& pos)
{
	lightPos_ = pos;
}

void MCB::SpotLight::SetSLightAtten(const Float3& atten)
{
	lightAtten_ = atten;
}

void MCB::SpotLight::SetIsActive(bool active)
{
	active_ = active;
}

void MCB::SpotLight::SetSLightFactorAngle(const Float2& factorCos,bool convertFlag)
{
	if ( convertFlag )
	{
		lightFactorAngleCos_.x_ = ConvertRadius(factorCos.x_);
		lightFactorAngleCos_.y_ = ConvertRadius(factorCos.y_);
	}
	else
	{
		lightFactorAngleCos_ = factorCos;
	}
}

void MCB::SpotLight::SetSLightShininess(float shininess)
{
	shininess_ = shininess;
}

 float MCB::SpotLight::GetSLightShininess()
{
	return shininess_;
}

 Vector3D MCB::SpotLight::GetSLightDir()
 {
	 return ligntDir_;
 }

 Float3 MCB::SpotLight::GetSLightColor()
 {
	 return lightColor_;
 }

 Float3 MCB::SpotLight::GetSLightPos()
 {
	 return lightPos_;
 }

 Float3 MCB::SpotLight::GetSLightAtten()
 {
	 return lightAtten_;
 }

 size_t MCB::SpotLight::IsActive()
 {
	 return active_;
 }

 Float2 MCB::SpotLight::GetSLightFactorAngle()
 {
	 return lightFactorAngleCos_;
 }

