#include "Util.h"

float MCB::Util::TruncateToTens(float value)
{
	return std::trunc(value / 10) * 10;
}