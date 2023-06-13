#include "Quaternion.h"
#include <cmath>
#define PI 3.14159265358979323846264338327950288f
using namespace MCB;

MCB::Quaternion::Quaternion(const Vector3D& vec, const float& angle)
{
	this->SetRota(vec, angle);
}

void MCB::Quaternion::SetRota( Vector3D vec, const float& angle)
{
	vec.V3Norm();
	float s = sinf(angle / 2);
	x_ = s * vec.vec_.x_;
	y_ = s * vec.vec_.y_;
	z_ = s * vec.vec_.z_;
	w_ = cosf(angle / 2);
	this->Normalize();
}

MCB::Quaternion::Quaternion()
{
	this->Identity();
}

MCB::Quaternion::Quaternion(const float& x, const float& y, const float& z, const float& w)
{
	this->x_ = x;
	this->y_ = y;
	this->z_ = z;
	this->w_ = w;
}

Quaternion MCB::Quaternion::GetConjugated(Quaternion q)
{
	q.x_ *= -1;
	q.y_ *= -1;
	q.z_ *= -1;
	return q;
}

Quaternion MCB::Quaternion::GetReciprocal( Quaternion q)
{
	Quaternion tempQ = GetConjugated(q);
	double norm = q.GetNorm();
	norm *= norm;
	tempQ = { tempQ.x_ / (float)norm, tempQ.y_ / (float)norm, tempQ.z_ / (float)norm, tempQ.w_ / (float)norm };
	return tempQ;
}

double MCB::Quaternion::GetNorm()
{
	return sqrt(x_ * x_ + y_ * y_ + z_ * z_ + w_ * w_);
}

Quaternion MCB::Quaternion::GetDirectProduct(const Quaternion& q,const Quaternion& p)
{
	Quaternion ans;
	ans.x_ = (q.x_ * p.w_) + (q.y_ * p.z_) - (q.z_ * p.y_) + (q.w_ * p.x_);
	ans.y_ = (-q.x_ * p.z_) + (q.y_ * p.w_) + (q.z_ * p.x_) + (q.w_ * p.y_);
	ans.z_ = (q.x_ * p.y_) - (q.y_ * p.x_) + (q.z_ * p.w_) + (q.w_ * p.z_);
	ans.w_ = (-q.x_ * p.x_) - (q.y_ * p.y_) - (q.z_ * p.z_) + (q.w_ * p.w_);
	ans.Normalize();
	return ans;
}

float MCB::Quaternion::Dot(const Quaternion& a, const Quaternion& b)
{
	
	return a.x_ * b.x_ + a.y_ * b.y_ + a.z_ * b.z_ + a.w_ * b.w_;
}

float MCB::Quaternion::GetAngle(const Quaternion& a, const Quaternion& b,float& dot,bool& nan)
{
	dot = Dot(a, b);
	if (dot < 0)
	{
		dot *= -1;
		nan = true;
	}
	return acosf(dot);
}

float MCB::Quaternion::GetAngle(const Quaternion& a, const Quaternion& b)
{
	float dot = Dot(a, b);
	if (dot < 0)
	{
		dot *= -1;
	}
	return acosf(dot);
}

Vector3D MCB::Quaternion::SetRotationVector(const Vector3D& rotationAxisVec,Vector3D PositionVec,const float& angle)
{
	PositionVec.V3Norm();
	Quaternion position{};
	position.x_ = PositionVec.vec_.x_;
	position.y_ = PositionVec.vec_.y_;
	position.z_ = PositionVec.vec_.z_;
	position.w_ = 0;
	Quaternion RotationAngle;
	RotationAngle.SetRota(rotationAxisVec, angle);

	position = GetDirectProduct(RotationAngle, position);
	position = GetDirectProduct(position, GetReciprocal(RotationAngle));

	position.Normalize();
	return Vector3D(position.x_,position.y_,position.z_);
}

Vector3D MCB::Quaternion::SetRotationVector(const Quaternion& rotationQuaternion, Vector3D PositionVec)
{
	PositionVec.V3Norm();
	Quaternion position{};
	position.x_ = PositionVec.vec_.x_;
	position.y_ = PositionVec.vec_.y_;
	position.z_ = PositionVec.vec_.z_;
	position.w_ = 0;

	position = GetDirectProduct(rotationQuaternion, position);
	position = GetDirectProduct(position, GetReciprocal(rotationQuaternion));

	position.Normalize();
	return Vector3D(position.x_, position.y_, position.z_);
}

Vector3D MCB::Quaternion::SetRotationVector(const Quaternion& rotationQuaternion, const Quaternion& PositionVec)
{

	Quaternion position{};

	position = GetDirectProduct(rotationQuaternion, PositionVec);
	position = GetDirectProduct(PositionVec, GetReciprocal(rotationQuaternion));

	position.Normalize();
	return Vector3D(position.x_, position.y_, position.z_);
}

Quaternion MCB::Quaternion::DirToDir( Vector3D u, Vector3D v)
{
	u.V3Norm();
	v.V3Norm();
	float angle = u.GetV3Dot(v);
	Vector3D axis = u.GetV3Cross(v);
	axis.V3Norm();
	float theta = acosf(angle);
	return Quaternion(axis,theta);
}


void MCB::Quaternion::Normalize()
{
	float mag = (float)sqrt(x_ * x_ + y_ * y_ + z_ * z_ + w_ * w_);

	if (mag > 0.0f)
	{
		float oneOverMag = 1.0f / mag;

		x_ *= oneOverMag;
		y_ *= oneOverMag;
		z_ *= oneOverMag;
		w_ *= oneOverMag;
	}
	

}

MCBMatrix MCB::Quaternion::GetQuaternionRotaMat(const Quaternion& rotaQ)
{
	MCBMatrix mat;
	mat.MCBMatrixIdentity();
	mat._11_ = 1 - (2 * (y_ * y_)) - (2 * (z_ * z_));
	mat._12_ = (2 * x_ * y_) + (2 * w_ * z_);
	mat._13_ = (2 * x_ * z_) - (2 * w_ * y_);
	mat._14_ = 0;

	mat._21_ = (2 * x_ * y_) - (2 * w_ * z_);
	mat._22_ = 1 - (2 * (x_ * x_)) - (2 * (z_ * z_));
	mat._23_ = (2 * y_ * z_) + (2 * w_ * x_);
	mat._24_ = 0;

	mat._31_ = (2 * x_ * z_) + (2 * w_ * y_);
	mat._32_ = (2 * y_ * z_) - (2 * w_ * x_);
	mat._33_ = 1 - (2 * (x_ * x_)) - (2 * (y_ * y_));
	mat._34_ = 0;

	mat._41_ = 0;
	mat._42_ = 0;
	mat._43_ = 0;
	mat._44_ = 1;


	return mat;
}

Quaternion MCB::Quaternion::Normalize( Quaternion q)
{
	q.Normalize();
	return q;
}

void MCB::Quaternion::SinCos(float* returnSin, float* returnCos, float theta)
{
	*returnSin = sin(theta);
	*returnCos = cos(theta);
}

Quaternion MCB::Quaternion::SetToRorateObjectToInternal(const Float3 eulerAngle)
{
	Quaternion ans;
	float sp, sb, sh;
	float cp, cb, ch;

	SinCos(&sp, &cp, eulerAngle.x * 0.5f);
	SinCos(&sb, &cb, eulerAngle.z * 0.5f);
	SinCos(&sh, &ch, eulerAngle.y * 0.5f);

	ans.x = ch * sp * cb + sh * cp * sb;
	ans.y = -ch * sp * sb + sh * cp * cb;
	ans.z = -sh * sp * cb + ch * cp * sb;
	ans.w = ch * cp * cb + sh * sp * sb;

	return ans;
}

Vector3D MCB::Quaternion::GetRotationAxis(Quaternion q)
{
	float sinThetaOver2Sq = 1.0f - q.w * q.w;

	if (sinThetaOver2Sq <= 0.0f)
	{
		return Vector3D(0,0,1);
	}


	float oneOverSinThetaOver2 = 1.0f / sqrt(sinThetaOver2Sq);


	return Vector3D(
		q.x * oneOverSinThetaOver2,
		q.y * oneOverSinThetaOver2,
		q.z * oneOverSinThetaOver2
		);

}

void MCB::Quaternion::GetRotationAxis(Quaternion q, Vector3D& AxisVec)
{
	float sinThetaOver2Sq = 1.0f - q.w * q.w;

	if (sinThetaOver2Sq <= 0.0f)
	{
		return;
	}


	float oneOverSinThetaOver2 = 1.0f / sqrt(sinThetaOver2Sq);


	AxisVec = Vector3D(
		q.x * oneOverSinThetaOver2,
		q.y * oneOverSinThetaOver2,
		q.z * oneOverSinThetaOver2
	);
}

float MCB::Quaternion::GetAngle(Quaternion q)
{
	float thetaOver2 = SafeAcos(q.w);
	return thetaOver2 * 2.0f;
}

float MCB::Quaternion::SafeAcos(float a)
{
	if (a <= -1.0f)
	{
		return PI;
	}
	if (a >= 1.0f)
	{
		return 0.0f;
	}
	return acos(a);
}

bool MCB::Quaternion::operator==(Quaternion q)
{
	if (q.x == x && q.y == y && q.z == z && q.w == w)
	{
		return true;
	}
	return false;
}

Quaternion MCB::Quaternion::operator-()
{
	return {-x,-y,-z,-w};
}

Quaternion MCB::Quaternion::Identity()
{
	return Quaternion(0,0,0,1);
}

MCB::Quaternion MCB::Quaternion::Slerp(Quaternion start, Quaternion end, int32_t time, int32_t maxTime)
{
	float Time = (float)time / (float)maxTime;
	Quaternion ans;
	Quaternion startDemo = start;
	float dot;
	bool isNan = false;
	float angle = GetAngle(start, end, dot,isNan);
	if (isNan)
	{
		startDemo = -start;
	}
	if (dot >= 1.0f - FLT_EPSILON)
	{
		ans.x = (1.f - Time) * startDemo.x + Time * end.x;
		ans.y = (1.f - Time) * startDemo.y + Time * end.y;
		ans.z = (1.f - Time) * startDemo.z + Time * end.z;
		ans.w = (1.f - Time) * startDemo.w + Time * end.w;
		ans.Normalize();
		return ans;
	}

	float st = sinf(angle);

	if (st == 0)
	{
		return -startDemo;
	}

	float sut = sinf(angle * Time);
	float sout = sinf(angle * (1.0f - Time));

	float coeff1 = sout / st;
	float coeff2 = sut / st;

	ans.x = coeff1 * start.x + coeff2 * end.x;
	ans.y = coeff1 * start.y + coeff2 * end.y;
	ans.z = coeff1 * start.z + coeff2 * end.z;
	ans.w = coeff1 * start.w + coeff2 * end.w;

	ans.Normalize();
	return ans;
}


MCB::Quaternion MCB::Quaternion::Slerp(Quaternion start, Quaternion end, float time)//ŒW”‚ð’¼‚Å“ü—Í‚·‚é—p
{
	Quaternion ans;
	float dot;
	Quaternion startDemo = start;
	bool isNan = false;
	float angle = GetAngle(start, end, dot, isNan);
	if (isNan)
	{
		startDemo = -start;
	}

	if (dot >= 1.0f - FLT_EPSILON)
	{
		ans.x = (1.f - time) * startDemo.x + time * end.x;
		ans.y = (1.f - time) * startDemo.y + time * end.y;
		ans.z = (1.f - time) * startDemo.z + time * end.z;
		ans.w = (1.f - time) * startDemo.w + time * end.w;
		ans.Normalize();
		return ans;
	}

	float st = sinf(angle);

	if (st == 0)
	{
		return -start;
	}

	float sut = sinf(angle * time);
	float sout = sinf(angle * (1.0f - time));

	float coeff1 = sout / st;
	float coeff2 = sut / st;

	ans.x = coeff1 * start.x + coeff2 * end.x;
	ans.y = coeff1 * start.y + coeff2 * end.y;
	ans.z = coeff1 * start.z + coeff2 * end.z;
	ans.w = coeff1 * start.w + coeff2 * end.w;

	ans.Normalize();
	return ans;
}

Quaternion MCB::SetRota(Vector3D vec, float angle)
{
	return Quaternion(vec, angle);
}
