#include "OBJSimpleHeader.hlsli"

VSOutput main( float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	//float3 lightdir = float3(1, -1, 1);
	//lightdir = normalize(lightdir);
	//float3 lightcolor = float3(1, 1, 1);

	float4 wnormal = (mul(world, float4(normal, 0)));
	float4 wpos = mul(world, pos);
    float4x4 wcMat = mul(cameraMat,world);

	VSOutput output;
    output.svpos = mul(mul(viewproj, wcMat), pos);
	output.worldpos = wpos;
    output.normal = normalize(wnormal.xyz);
	output.uv = uv;

	return output;
}