#include "FbxShaderHeader.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

PSOutput main(GSOutput input)
{
    PSOutput output;
    float4 texcolor = float4(tex.Sample(smp, input.uv));
    float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
    float3 ambient = m_ambient;
    float4 shadeColor = float4(ambientColor * ambient, m_alpha) * texcolor * color;
    for (int i = 0; i < DIRLIGHT_NUM; i++)
    {
        if (dirLights[i].active)
        {
            float3 dotlightnormal = saturate(dot(dirLights[i].lightv, input.normal));
            float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
            float3 diffuse = dotlightnormal * m_diffuse * texcolor.rgb * color.rgb;
            float3 speculer = pow(saturate(dot(reflect, eyedir)), dirLights[i].shininess) * m_specular;
            float3 color = saturate((diffuse + speculer) * dirLights[i].lightcolor);
            shadeColor.rgb += color.rgb;

        }
    }

    for (int j = 0; j < PLIGHT_NUM; j++)
    {
        if (pLights[j].active)
        {
            float3 lightVec = pLights[j].lightPos - input.worldpos.xyz;
            float d = length(lightVec);
            lightVec = normalize(lightVec);
            float atten = 1.0f / (pLights[j].lightAtten.x + pLights[j].lightAtten.y * d + pLights[j].lightAtten.z * d * d);
            float3 dotLightNormal = saturate(dot(lightVec, input.normal));
            float3 reflect = normalize(-lightVec + 2 * dotLightNormal * input.normal);
            float3 diffuse = dotLightNormal * m_diffuse * texcolor.rgb * color.rgb;
            float3 specular = pow(saturate(dot(reflect, eyedir)), pLights[j].shininess) * m_specular;
            float3 color = (saturate(diffuse + specular) * pLights[j].lightColor);
            shadeColor.rgb += color.rgb;
        }
    }
	
    for (int k = 0; k < SLIGHT_NUM; k++)
    {
        if (sLights[k].active)
        {
            float3 lightVec = sLights[k].lightPos - input.worldpos.xyz;
            float d = length(lightVec);
            lightVec = normalize(lightVec);
            float atten = saturate(1.0f / (sLights[k].lightAtten.x + sLights[k].lightAtten.y * d + sLights[k].lightAtten.z * d * d));
            float cos = dot(lightVec, sLights[k].ligntVec);
            float angleAtten = smoothstep(sLights[k].lightFactorAngleCos.y, sLights[k].lightFactorAngleCos.x, cos);
            atten *= angleAtten;
            float3 dotLightNormal = saturate(dot(lightVec, input.normal));
            float3 reflect = normalize(-lightVec + 2 * dotLightNormal * input.normal);
            float3 diffuse = dotLightNormal * m_diffuse * texcolor.rgb * color.rgb;
            float3 specular = pow(saturate(dot(reflect, eyedir)), sLights[k].shininess) * m_specular;
            float3 color = saturate((diffuse + specular) * sLights[k].lightColor);
            shadeColor.rgb += color.rgb;

        }
    }
    output.target0 = shadeColor;
    output.target1 = output.target0;
    
    return output;
}
