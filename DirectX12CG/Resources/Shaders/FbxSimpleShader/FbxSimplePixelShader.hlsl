#include "FbxSimpleShaderHeader.hlsli"
PSOutput main(GSOutput input)
{
    PSOutput output;
    output.target0 = color;
    output.target1 = color;
    return output;
}


