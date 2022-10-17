#include "ParticleHeader.hlsli"
static const uint vnum = 4;

static const float4 offSetArray[vnum] =
{
    float4(-0.5f, -0.5f, 0, 0),
    float4(-0.5f, +0.5f, 0, 0),
    float4(+0.5f, -0.5f, 0, 0),
    float4(+0.5f, +0.5f, 0, 0)
};

static const float2 uvArray[vnum] =
{
    float2(0, 1),
    float2(0, 0),
    float2(1, 1),
    float2(1, 0)
};


[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : SV_POSITION,
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element;
    for (uint i = 0; i < vnum; i++)
    {
        element.svpos = input[0].svpos + offSetArray[i];
        element.svpos = mul(mul(viewproj, world), element.svpos);
        element.uv = uvArray[i];
        output.Append(element);
    }
	
}