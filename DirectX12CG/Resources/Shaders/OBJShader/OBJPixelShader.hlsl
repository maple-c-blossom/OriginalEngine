#include "OBJHeader.hlsli"

Texture2D<float4> tex:register(t0);
SamplerState smp:register(s0);

float4 main(GSOutput input) : SV_TARGET
{
	float4 texcolor = float4(tex.Sample(smp, input.uv));
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	float3 ambient = m_ambient;
	float4 shadeColor = float4(ambientColor * ambient, m_alpha);
    float shininess = 3.0f;
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

	for (int i = 0; i < PLIGHT_NUM; i++)
	{
		if (pLights[i].active)
		{
			float3 lightVec = pLights[i].lightPos - input.worldpos.xyz;
			float d = length(lightVec);
			lightVec = normalize(lightVec);
            float atten = 1.0f / (pLights[i].lightAtten.x + pLights[i].lightAtten.y * d + pLights[i].lightAtten.z * d * d);
            float3 dotLightNormal = dot(lightVec, input.normal);
            float3 reflect = normalize(-lightVec + 2 * dotLightNormal * input.normal);
            float3 diffuse = dotLightNormal * m_diffuse;
            float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
            shadeColor.rgb += atten * (diffuse + specular) * pLights[i].lightColor;
        }
	}
	return shadeColor * texcolor * color;
}
