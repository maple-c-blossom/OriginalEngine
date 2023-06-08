#pragma once
#include <math.h>
#include "Object3d.h"
#define PI 3.14159265358979323846264338327950288f
namespace MCB
{
	double Lerp(const double& startPos, const double& endPos, const double& maxTime, const double& time);
	double Lerp(const double& startPos, const double& endPos, const double& time);
	double InQuad(const double& startPos, const double& endPos, const double& maxTime, const double& time);
	double OutQuad(const double& startPos, const double& endPos, const double& maxTime, const double& time);
	double InOutQuad(const double& startPos, const double& endPos, const double& maxTime, const double& time);
	double OutInRelayQuad(const double& startPos, const double& endPos, const double& relayPos,const  double& maxTime, const double& relayTime, const double& time);
	double OutOutRelayQuad(const double& startPos, const double& endPos, const double& relayPos, const double& maxTime, const double& relayTime, const double& time);
	double EaseInBack(const double& startPos, const double& endPos, const double& maxTime,const  double& time);
	double EaseInBack2(const double& startPos, const double& endPos, const double& maxTime, const double& time);
	double EaseOutBounce(const double& startPos, const double& endPos, const double& time, const double& maxTime);
	int32_t Abs(const int32_t& num);
	float Abs(const float& num);
	double Abs(const double& num);
	float ConvertRadius(const float& angle);

	void InitRand();
	int32_t GetRand(const int32_t& min, const int32_t& max);
	float clamp(const float& f);

	class SimpleFigure
	{
	public:
		SimpleFigure();
		Model triangleMaterial;
		Object3d triangle;

		Float3 PointA = { -1.5f,-0.5f,4.0f }; Float3 PointB = { 4.5f,0.5f,1.0f };
		Float3 PointC = { 0.5f,-3.5f,-4.0f }; Float4 color = { 1.0f,1.0f,1.0f,1.0f };
		void  DrawTriangle(ICamera* camera);

	};
}

