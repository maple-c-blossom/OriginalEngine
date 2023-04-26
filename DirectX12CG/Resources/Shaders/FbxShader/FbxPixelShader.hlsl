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

    for (int i = 0; i < PLIGHT_NUM; i++)
    {
        if (pLights[i].active)
        {
            float3 lightVec = pLights[i].lightPos - input.worldpos.xyz;
            float d = length(lightVec);
            lightVec = normalize(lightVec);
            float atten = 1.0f / (pLights[i].lightAtten.x + pLights[i].lightAtten.y * d + pLights[i].lightAtten.z * d * d);
            float3 dotLightNormal = saturate(dot(lightVec, input.normal));
            float3 reflect = normalize(-lightVec + 2 * dotLightNormal * input.normal);
            float3 diffuse = dotLightNormal * m_diffuse * texcolor.rgb * color.rgb;
            float3 specular = pow(saturate(dot(reflect, eyedir)), pLights[i].shininess) * m_specular;
            float3 color = (saturate(diffuse + specular) * pLights[i].lightColor);
            shadeColor.rgb += color.rgb;
        }
    }
	
    for (int i = 0; i < SLIGHT_NUM; i++)
    {
        if (sLights[i].active)
        {
            float3 lightVec = sLights[i].lightPos - input.worldpos.xyz;
            float d = length(lightVec);
            lightVec = normalize(lightVec);
            float atten = saturate(1.0f / (sLights[i].lightAtten.x + sLights[i].lightAtten.y * d + sLights[i].lightAtten.z * d * d));
            float cos = dot(lightVec, sLights[i].ligntVec);
            float angleAtten = smoothstep(sLights[i].lightFactorAngleCos.y, sLights[i].lightFactorAngleCos.x, cos);
            atten *= angleAtten;
            float3 dotLightNormal = saturate(dot(lightVec, input.normal));
            float3 reflect = normalize(-lightVec + 2 * dotLightNormal * input.normal);
            float3 diffuse = dotLightNormal * m_diffuse * texcolor.rgb * color.rgb;
            float3 specular = pow(saturate(dot(reflect, eyedir)), sLights[i].shininess) * m_specular;
            float3 color = saturate((diffuse + specular) * sLights[i].lightColor);
            shadeColor.rgb += color.rgb;

        }
    }
    output.target0 = shadeColor;
    output.target1 = output.target0;
    
    return output;
}
