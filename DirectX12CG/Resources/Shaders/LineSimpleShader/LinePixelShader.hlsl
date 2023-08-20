#include "LineHeader.hlsli"


PSOutput main(VSOutput input)
{
    PSOutput output;
    output.target0 = color;
    output.target1 = color;
    return output;
}


