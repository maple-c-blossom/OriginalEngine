#include "MathUtil.h"

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
