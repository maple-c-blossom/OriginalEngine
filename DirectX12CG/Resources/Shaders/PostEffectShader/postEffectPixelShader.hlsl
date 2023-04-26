#include "PostEffectShaderHeader.hlsli"

Texture2D<float4> tex0 : register(t0);
Texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float4 texColor0 = tex0.Sample(smp, input.uv);
    float4 texColor1 = tex1.Sample(smp, input.uv);
    float4 finalColor = texColor0;
    if(fmod(input.uv.y,0.1f) < 0.05f)
    {
        finalColor = texColor1;
    }
    return finalColor;
}