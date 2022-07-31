#include "OBJHeader.hlsli"

Texture2D<float4> tex:register(t0);
SamplerState smp:register(s0);

float4 main(GSOutput input) : SV_TARGET
{
	//float3 light = normalize(float3(1,-1,1));
	//float light_diffuse = saturate(dot(-light, input.normal));
	//float3 shader_color;
	//shader_color = m_ambient;
	//shader_color += m_diffuse * light_diffuse;
	float4 shadercolor;

	float4 texcolor = float4(tex.Sample(smp, input.uv));
	const float shininess = 4.0f;
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	float3 dotlightnormal = dot(lightv, input.normal);
	float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
	float3 ambient = m_ambient;
	float3 diffuse = dotlightnormal * m_diffuse;
	float3 speculer = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
	shadercolor.rgb = (ambient + diffuse + speculer) * lightcolor;
	shadercolor.a = m_alpha;
	return shadercolor * texcolor * color;
}