#include "PostEffectShaderHeader.hlsli"

Texture2D<float4> tex0 : register(t0);
Texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    if (fmod(input.uv.y, 0.1f) < 0.05f)
    {
        float uChange = 1 / 1280.f;
        float vChange = 1 / 720.f;
        float4 texColor1sum = { 0, 0, 0, 0 };
        float uForRange = 3;
        float vForRange = 3;
        for (float v = 0; v < vForRange; v++)
        {
            for (float u = 0; u < uForRange; u++)
            {
                float2 offSet = ((u - 1) * uChange, (v - 1) * vChange);
                texColor1sum += tex1.Sample(smp, saturate(input.uv - offSet));
            }
        }
        return texColor1sum / (uForRange * vForRange);
    }
    float4 texColor0 = float4(1 - tex0.Sample(smp, input.uv).rgb, 1);
    float4 texColor1 = float4(tex1.Sample(smp, input.uv));
    return texColor1;
}