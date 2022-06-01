#include "Quaternion.h"
#include <cmath>
using namespace MCB;

void MCB::Quaternion::SetRota(Vector3D vec, float angle)
{
	vec.V3Norm();
	float s = (sinf(angle) / 2);
	x = vec.vec.x * s;
	y = vec.vec.y * s;
	z = vec.vec.z * s;
	w = cosf(angle) / 2;
}

Quaternion MCB::Quaternion::GetConjugated(Quaternion q)
{
	q.x *= -1;
	q.y *= -1;
	q.z *= -1;
	return q;
}

Quaternion MCB::Quaternion::GetReciprocal(Quaternion q)
{
	return GetConjugated(q);
}

Quaternion MCB::Quaternion::GetCartesianProduct(Quaternion q, Quaternion p)
{
	Quaternion ans;
	ans.x = (q.w * p.x) - (q.z * p.y) + (q.y * p.z) + (q.x * p.w);
	ans.y = (q.z * p.x) + (q.w * p.y) + (q.x * p.z) + (q.y * p.w);
	ans.z = -(q.y * p.x) + (q.x * p.y) + (q.w * p.z) + (q.z * p.w);
	ans.w = -(q.x * p.x) - (q.y * p.y) - (q.z * p.z) + (q.w * p.w);
	return ans;
}

float MCB::Quaternion::Dot(Quaternion a, Quaternion b)
{
	
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

float MCB::Quaternion::GetAngle(Quaternion a, Quaternion b)
{
	return acosf(Dot(a, b));;
}

Quaternion MCB::Quaternion::SetRotationQuaternion(Vector3D rotationAxisVec, Vector3D PositionVec, float angle)
{
	Quaternion position;
	position.x = PositionVec.vec.x;
	position.y = PositionVec.vec.y;
	position.z = PositionVec.vec.z;
	position.w = 0;
	Quaternion RotationAngle;
	RotationAngle.SetRota(rotationAxisVec, angle);

	position = GetCartesianProduct(GetReciprocal(RotationAngle), position);
	position = GetCartesianProduct(position, RotationAngle);

	return position;
}

MCBMatrix MCB::Quaternion::GetQuaternionRotaMat(Quaternion rotaQ)
{
	MCBMatrix mat;
	mat.MCBMatrixIdentity();
	mat._11 = 1 - (2 * (y * y)) - (2 * (z * z));
	mat._12 = (2 * x * y) + (2 * w * z);
	mat._13 = (2 * x * z) - (2 * w * y);
	mat._14 = 0;

	mat._21 = (2 * x * y) - (2 * w * z);
	mat._22 = 1 - (2 * (x * x)) - (2 * (z * z));
	mat._23 = (2 * y * z) + (2 * w * x);
	mat._24 = 0;

	mat._31 = (2 * x * z) + (2 * w * y);
	mat._32 = (2 * y * z) - (2 * w * x);
	mat._33 = 1 - (2 * (x * x)) - (2 * (y * y));
	mat._34 = 0;

	mat._41 = 0;
	mat._42 = 0;
	mat._43 = 0;
	mat._44 = 1;


	return mat;
}

MCB::Quaternion MCB::Quaternion::Slerp(Quaternion start, Quaternion end, int time, int maxTime)
{
	time = time / maxTime;
	Quaternion ans;
	float angle = GetAngle(start, end);

	if (angle < 0)
	{
		angle *= -1;
	}


	float st = sinf(angle);

	if (st == 0)
	{
		return start;
	}

	float sut = sinf(angle * time);
	float sout = sinf(angle * (1.0f - time));

	float coeff1 = sout / st;
	float coeff2 = sut / st;

	ans.x = coeff1 * start.x + coeff2 * end.x;
	ans.y = coeff1 * start.y + coeff2 * end.y;
	ans.z = coeff1 * start.z + coeff2 * end.z;
	ans.w = coeff1 * start.w + coeff2 * end.w;

	return ans;
}
