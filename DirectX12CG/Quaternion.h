#pragma once
#include "Vector3D.h"
namespace MCB
{
	class Quaternion
	{
		float x;
		float y;
		float z;
		float w;

		void SetRota(Vector3D vec,float angle);
		Quaternion GetConjugated(Quaternion q);
		Quaternion GetReciprocal(Quaternion q);
		Quaternion GetCartesianProduct(Quaternion q, Quaternion p);
		float Dot(Quaternion a, Quaternion b);
		float GetAngle(Quaternion a, Quaternion b);
		Quaternion Slerp(Quaternion start, Quaternion end, int time,int maxTime);
		Quaternion SetRotationQuaternion(Vector3D rotationAxisVec, Vector3D PositionVec, float angle);
	};
}

