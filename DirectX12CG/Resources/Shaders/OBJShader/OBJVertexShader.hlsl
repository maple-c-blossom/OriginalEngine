#include "OBJHeader.hlsli"

VSOutput main( float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	float3 lightdir = float3(1, -1, 1);
	lightdir = normalize(lightdir);
	float3 lightcolor = float3(1, 1, 1);
	float3 ambient = m_ambient;
	float3 diffuse = dot(lightdir,normal) * m_diffuse;
	const float3 eye = float3(0, 0, -20);
	const float shininess = 4.0f;
	float eyedir = normalize(eye - pos.xyz);
	float reflect = normalize(lightdir + 2 * dot(-lightdir, normal) * normal);
	float speculer = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

	VSOutput output;
	output.svpos = mul(mat, pos);
	output.color.rgb = (ambient + diffuse + speculer) * lightcolor;
	output.color.a = m_alpha;
	output.uv = uv;

	return output;
}