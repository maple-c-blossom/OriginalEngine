#include "Vector3.h"
#include <cmath>


using namespace MCB;
MCB::Vector3::Vector3(const Vector3& start, const Vector3& end)
{
	Vector3 temp = end - start;
	x = temp.x;
	y = temp.y;
	z = temp.z;
}

MCB::Vector3::Vector3()
{
	x = 0;
	y = 0;
	z = 0;
}

MCB::Vector3::Vector3(float x, float y, float z)
{
	x = x;
	y = y;
	z = z;
}
Vector3 MCB::Vector3::Vector3Substruct(const Vector3& start, const Vector3& end)
{
	Vector3 temp(start, end);
	return temp;
}

float MCB::Vector3::V3Len() const
{
	return sqrtf(((x * x) + (y * y) + (z * z)));
}

void MCB::Vector3::V3Norm()
{
	float VecLen = V3Len();
	if (VecLen == 0.0f)
	{
		//assert(0);
		return;
	}

	x = x / VecLen;
	y = y / VecLen;
	z = z / VecLen;

}

Vector3 MCB::Vector3::GetV3Norm()
{
	Vector3 ans = *this;
	ans.V3Norm();
	return ans;
}

float MCB::Vector3::GetV3Dot(const Vector3& vector)
{
	return (x * vector.x) + (y * vector.y) + (z * vector.z);
}

Vector3 MCB::Vector3::GetV3Cross(const Vector3& vector)
{
	Vector3 temp;
	temp.x = y * vector.z - z * vector.y;
	temp.y = z * vector.x - x * vector.z;
	temp.z = x * vector.y - y * vector.x;
	return temp;
}

Vector3 MCB::Vector3::GetV3Cross(const Vector3& avector, const Vector3& bvector)
{
	Vector3 temp;
	temp.x = avector.y * bvector.z - avector.z * bvector.y;
	temp.y = avector.z * bvector.x - avector.x * bvector.z;
	temp.z = avector.x * bvector.y - avector.y * bvector.x;
	return temp;
}

Vector3 MCB::Vector3::GetUpVec(Vector3 RightVec, Vector3 frontVec)
{
	Vector3 ans;
	RightVec.V3Norm();
	frontVec.V3Norm();
	ans = ans.GetV3Cross(frontVec, RightVec);
	ans.V3Norm();

	return ans;
}


Vector3 MCB::Vector3::GetRightVec(Vector3 frontVec, Vector3 UpVec)
{
	Vector3 ans;
	frontVec.V3Norm();
	UpVec.V3Norm();
	ans = ans.GetV3Cross(UpVec, frontVec);
	ans.V3Norm();

	return ans;
}

Vector3 MCB::Vector3::GetV3Normal(Vector3 v0, Vector3 v1, Vector3 v2)
{
	Vector3 ans;
	Vector3 vv1, vv2;
	Vector3 v0n, v1n, v2n;
	v0n = v0; //v0n.V3Norm();
	v1n = v1; //v1n.V3Norm();
	v2n = v2; //v2n.V3Norm();
	vv1 = vv1.Vector3Substruct(v0n, v1n);
	vv2 = vv2.Vector3Substruct(v1n, v2n);
	vv1.V3Norm();
	vv2.V3Norm();
	ans = vv1.GetV3Cross(vv2);
	ans.V3Norm();
	return ans;
}

Vector3 MCB::Vector3::Qlamp(const Vector3& vec, const Vector3& min, const Vector3& max)
{
	Vector3 ret = vec;
	if (vec.x < min.x)
	{
		ret.x = min.x;
	}
	else if (vec.x > max.x)
	{
		ret.x = max.x;
	}

	if (vec.y < min.y)
	{
		ret.y = min.y;
	}
	else if (vec.y > max.y)
	{
		ret.y = max.y;
	}

	if (vec.z < min.z)
	{
		ret.z = min.z;
	}
	else if (vec.z > max.z)
	{
		ret.z = max.z;
	}

	return ret;
}



float MCB::Vector3::GetInnerProduct(Vector3 vector)
{
	Vector3 temp;
	temp = *this;
	temp.V3Norm();
	vector.V3Norm();
	return acos(temp.GetV3Dot(vector));
}

Vector3& MCB::Vector3::operator+=(const Vector3& Vec)
{
	x += Vec.x;
	y += Vec.y;
	z += Vec.z;
	return *this;
}

Vector3& MCB::Vector3::operator-=(const Vector3& Vec)
{
	x -= Vec.x;
	y -= Vec.y;
	z -= Vec.z;
	return *this;
}

Vector3& MCB::Vector3::operator*=(float k)
{
	x *= k;
	y *= k;
	z *= k;
	return *this;
}

Vector3 MCB::Vector3::Clamp(const Vector3& vec1, const Vector3& min, const Vector3& max, bool* isClamped)
{
	Vector3 ret;
	bool isClamp = false;
	if (vec1.x < min.x)
	{
		ret.x = min.x;
		isClamp = true;
	}
	else if (vec1.x > max.x)
	{
		ret.x = max.x;
		isClamp = true;
	}


	if (vec1.y < min.y)
	{
		ret.y = min.y;
		isClamp = true;
	}
	else if (vec1.y > max.y)
	{
		ret.y = max.y;
		isClamp = true;
	}

	if (vec1.z < min.z)
	{
		ret.z = min.z;
		isClamp = true;
	}
	else if (vec1.z > max.z)
	{
		ret.z = max.z;
		isClamp = true;
	}
	if (isClamped != nullptr)
	{
		*isClamped = isClamp;
	}

	return ret;
}

Vector3 MCB::operator+(const Vector3& vecA, const Vector3& vecB)
{
	Vector3 temp;
	temp = vecA;
	temp += vecB;
	return temp;
}

bool MCB::operator!=(const Vector3& vecA, const Vector3& vecB)
{
	return (vecA.x != vecB.x || vecA.y != vecB.x || vecA.z != vecB.x);
}

Vector3 MCB::operator-(const Vector3& vecA, const Vector3& vecB)
{
	Vector3 temp;
	temp = vecA;
	temp -= vecB;
	return temp;
}

Vector3 MCB::operator*(const Vector3& vecA, float k)
{
	Vector3 temp;
	temp = vecA;
	temp *= k;
	return temp;
}

Vector3 MCB::operator*(float k, const Vector3& vecA)
{
	Vector3 temp;
	temp = vecA;
	temp *= k;
	return temp;
}

Vector3 MCB::operator/(const Vector3& vecA, float k)
{
	Vector3 temp;
	temp.x = vecA.x / k;
	temp.y = vecA.y / k;
	temp.z = vecA.z / k;
	return temp;
}

Vector3 MCB::operator-(const Vector3& vecA)
{
	Vector3 temp;
	temp.x = vecA.x * -1;
	temp.y = vecA.y * -1;
	temp.z = vecA.z * -1;
	return temp;
}


