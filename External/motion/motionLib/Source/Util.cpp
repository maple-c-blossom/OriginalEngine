#include "Util.h"

float MCBM::Util::TruncateToTens(float value)
{
	return std::trunc(value / 10) * 10;
}