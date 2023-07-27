#pragma once
#include <math.h>
#include "Object3d.h"
#define PI 3.14159265358979323846264338327950288f
namespace MCB
{
	double Lerp( double startPos,  double endPos,  double maxTime,  double time);
	double Lerp( double startPos,  double endPos,  double time);
	double InQuad( double startPos,  double endPos,  double maxTime,  double time);
	double OutQuad( double startPos,  double endPos,  double maxTime,  double time);
	double InOutQuad( double startPos,  double endPos,  double maxTime,  double time);
	double OutInRelayQuad( double startPos,  double endPos,  double relayPos,  double maxTime, 
		double relayTime,  double time);
	double OutOutRelayQuad( double startPos,  double endPos,  double relayPos, 
		double maxTime,  double relayTime,  double time);
	double EaseInBack( double startPos,  double endPos,  double maxTime, double time);
	double EaseInBack2( double startPos,  double endPos,  double maxTime, double time);
	double EaseOutBounce( double startPos,  double endPos,  double time, double maxTime);
	int32_t Abs( int32_t num);
	float Abs( float num);
	double Abs( double num);
	float ConvertRadius( float angle);

	void InitRand();
	int32_t GetRand( int32_t min,  int32_t max);
	float clamp( float f);

	float clamp(float f, float min, float max);

	class SimpleFigure
	{
	public:
		SimpleFigure();
		Model triangleMaterial_;
		Object3d triangle_;

		Float3 PointA_ = { -1.5f,-0.5f,4.0f }; Float3 PointB_ = { 4.5f,0.5f,1.0f };
		Float3 PointC_ = { 0.5f,-3.5f,-4.0f }; Float4 color_ = { 1.0f,1.0f,1.0f,1.0f };
		void  DrawTriangle(ICamera* camera);

	};
}

