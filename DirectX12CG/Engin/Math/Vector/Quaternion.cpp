#include "Quaternion.h"
#include "IgnoreWarning.h"
WarningIgnoreBegin
#include <cmath>
WarningIgnoreEnd
#define PI 3.14159265358979323846264338327950288f
using namespace MCB;


MCB::Quaternion::Quaternion(const Vector3D& vec, float angle)
{
	SetRota(vec, angle);
}

void MCB::Quaternion::SetRota( Vector3D vec, float angle)
{
	vec.V3Norm();
	float s = sinf(angle / 2);
	x_ = s * vec.vec_.x_;
	y_ = s * vec.vec_.y_;
	z_ = s * vec.vec_.z_;
	w_ = cosf(angle / 2);
	Normalize();
}

MCB::Quaternion::Quaternion()
{
	Identity();
}

MCB::Quaternion::Quaternion(float x, float y, float z, float w)
{
	x_ = x;
	y_ = y;
	z_ = z;
	w_ = w;
}

MCB::Quaternion::Quaternion(DirectX::XMVECTOR q)
{
	x_ = q.m128_f32[0];
	y_ = q.m128_f32[1];
	z_ = q.m128_f32[2];
	w_ = q.m128_f32[3];
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
		//dot *= -1;
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

Vector3D MCB::Quaternion::SetRotationVector(const Vector3D& rotationAxisVec,Vector3D PositionVec, float angle)
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
	static_cast< void >( rotaQ );
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

Quaternion MCB::Quaternion::SetToRorateObjectToInternal(const Float3& eulerAngle)
{
	Quaternion ans;
	float sp, sb, sh;
	float cp, cb, ch;

	SinCos(&sp, &cp, eulerAngle.x_ * 0.5f);
	SinCos(&sb, &cb, eulerAngle.z_ * 0.5f);
	SinCos(&sh, &ch, eulerAngle.y_ * 0.5f);

	ans.x_ = ch * sp * cb + sh * cp * sb;
	ans.y_ = -ch * sp * sb + sh * cp * cb;
	ans.z_ = -sh * sp * cb + ch * cp * sb;
	ans.w_ = ch * cp * cb + sh * sp * sb;

	return ans;
}

Vector3D MCB::Quaternion::GetRotationAxis(const Quaternion& q)
{
	float sinThetaOver2Sq = 1.0f - q.w_ * q.w_;

	if (sinThetaOver2Sq <= 0.0f)
	{
		return Vector3D(0,0,1);
	}


	float oneOverSinThetaOver2 = 1.0f / sqrt(sinThetaOver2Sq);


	return Vector3D(
		q.x_ * oneOverSinThetaOver2,
		q.y_ * oneOverSinThetaOver2,
		q.z_ * oneOverSinThetaOver2
		);

}

void MCB::Quaternion::GetRotationAxis(const Quaternion& q, Vector3D& AxisVec)
{
	float sinThetaOver2Sq = 1.0f - q.w_ * q.w_;

	if (sinThetaOver2Sq <= 0.0f)
	{
		return;
	}


	float oneOverSinThetaOver2 = 1.0f / sqrt(sinThetaOver2Sq);


	AxisVec = Vector3D(
		q.x_ * oneOverSinThetaOver2,
		q.y_ * oneOverSinThetaOver2,
		q.z_ * oneOverSinThetaOver2
	);
}

float MCB::Quaternion::GetAngle(const Quaternion& q)
{
	float thetaOver2 = SafeAcos(q.w_);
	return thetaOver2 * 2.0f;
}

float MCB::Quaternion::SafeAcos( float a)
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

bool MCB::Quaternion::operator==(const Quaternion& q)
{
	if (q.x_ == x_ && q.y_ == y_ && q.z_ == z_ && q.w_ == w_)
	{
		return true;
	}
	return false;
}

Quaternion MCB::Quaternion::operator=(const DirectX::XMVECTOR& q)
{
	Quaternion temp;
	temp.x_ = q.m128_f32[0];
	temp.y_ = q.m128_f32[1];
	temp.z_ = q.m128_f32[2];
	temp.w_ = q.m128_f32[3];
	return temp;
}

Quaternion MCB::Quaternion::operator-()
{
	return {-x_,-y_,-z_,-w_};
}

Quaternion MCB::Quaternion::operator*(float k)
{
	return Quaternion(x_* k,y_ * k, z_ * k,w_ * k);
}

Quaternion MCB::Quaternion::operator+(Quaternion q)
{
	return Quaternion(x_ + q.x_, y_ + q.y_, z_ + q.z_, w_+ q.w_);
}

Quaternion MCB::operator*(float k, Quaternion q)
{
	return q * k;
}

Quaternion MCB::Quaternion::Identity()
{
	return Quaternion(0,0,0,1);
}

DirectX::XMVECTOR MCB::Quaternion::ConvertXMVector()
{
	return DirectX::XMVECTOR{ {x_,y_,z_,w_} };
}

MCB::Quaternion MCB::Quaternion::Slerp(Quaternion start,const Quaternion& end,
	 int32_t time, int32_t maxTime)
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
		ans.x_ = (1.f - Time) * startDemo.x_ + Time * end.x_;
		ans.y_ = (1.f - Time) * startDemo.y_ + Time * end.y_;
		ans.z_ = (1.f - Time) * startDemo.z_ + Time * end.z_;
		ans.w_ = (1.f - Time) * startDemo.w_ + Time * end.w_;
		ans.Normalize();
		return ans;
	}

	float st = sinf(angle);

	if (st == 0)
	{
		return -start;
	}

	float sut = sinf(angle * Time);
	float sout = sinf(angle * (1.0f - Time));

	float coeff1 = sout / st;
	float coeff2 = sut / st;

	ans.x_ = coeff1 * start.x_ + coeff2 * end.x_;
	ans.y_ = coeff1 * start.y_ + coeff2 * end.y_;
	ans.z_ = coeff1 * start.z_ + coeff2 * end.z_;
	ans.w_ = coeff1 * start.w_ + coeff2 * end.w_;

	ans.Normalize();
	return ans;
}


MCB::Quaternion MCB::Quaternion::Slerp( Quaternion start, Quaternion end, float time)//係数を直で入力する用
{
	if ( start.operator== (end) )
	{
		return start;
	}
	Quaternion ans;
	float dot;
	Quaternion endDemo = end;
	bool isNan = false;
	float angle = GetAngle(start, end, dot, isNan);
	if (isNan)
	{
		endDemo = -end;
	}

	if (dot >= 1.0f - FLT_EPSILON)
	{
		ans.x_ = (1.f - time) * start.x_ + time * endDemo.x_;
		ans.y_ = (1.f - time) * start.y_ + time * endDemo.y_;
		ans.z_ = (1.f - time) * start.z_ + time * endDemo.z_;
		ans.w_ = (1.f - time) * start.w_ + time * endDemo.w_;
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

	ans.x_ = coeff1 * start.x_ + coeff2 * endDemo.x_;
	ans.y_ = coeff1 * start.y_ + coeff2 * endDemo.y_;
	ans.z_ = coeff1 * start.z_ + coeff2 * endDemo.z_;
	ans.w_ = coeff1 * start.w_ + coeff2 * endDemo.w_;

	ans.Normalize();
	return ans;

}

Quaternion MCB::SetRota(const Vector3D& vec, float angle)
{
	return Quaternion(vec, angle);
}
