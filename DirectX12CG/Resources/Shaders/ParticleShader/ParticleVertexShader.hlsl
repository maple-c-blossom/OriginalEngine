#include "ParticleHeader.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	VSOutput output;
	output.svpos = pos;
	//output.uv = uv;
	

	return output;
}