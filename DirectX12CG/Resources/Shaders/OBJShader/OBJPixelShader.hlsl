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
	float4 texcolor = float4(tex.Sample(smp, input.uv));
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	float3 ambient = m_ambient;
	float4 shadeColor = float4(ambientColor * ambient, m_alpha);
	for (int i = 0; i < DIRLIGHT_NUM; i++)
	{
		if (dirLights[i].active)
		{
			float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);
			float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
			float3 diffuse = dotlightnormal * m_diffuse;
			float3 speculer = pow(saturate(dot(reflect, eyedir)), dirLights[i].shininess) * m_specular;
			shadeColor.rgb += (diffuse + speculer) * dirLights[i].lightcolor;
		}
	}
	return shadeColor * texcolor * color;
}
