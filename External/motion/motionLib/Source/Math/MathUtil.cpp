#include <Math/MathUtil.h>
#include <math.h>
#include <iostream>
float MCBM::MathUtil::Clamp(float var, float min, float max)
{
    if (var < min)
    {
        return min;
    }

    if (var > max)
    {
        return max;
    }
    return var;
}

float MCBM::Util::TruncateToTens(float value)
{
    return std::trunc(value / 10) * 10;
}