#include "PostEffectShaderHeader.hlsli"

Texture2D<float4> tex0 : register(t0);
Texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0);

float4 AverageBlur(VSOutput input, Texture2D<float4> tex)
{
    float4 texColor1 = float4(tex.Sample(smp, input.uv));
    //const float uChange = 1 / 1280.f;
    //const float vChange = 1 / 720.f;
    //float4 texColor1sum = { 0, 0, 0, 0 };
    //const float uForRange = 6;
    //const float vForRange = 6;
    //for (float v = 0; v < vForRange; v++)
    //{
    //    for (float u = 0; u < uForRange; u++)
    //    {
    //        float2 offSet = float2((u - 1) * uChange, (v - 1) * vChange);
    //        texColor1sum += tex.Sample(smp, saturate(input.uv - offSet));
    //    }
    //}
    return texColor1;
}

float4 inv(VSOutput input, Texture2D<float4> tex)
{
    return float4(1 - tex.Sample(smp, input.uv).rgb, 1);
}

float4 gray(VSOutput input,Texture2D<float4> tex)
{
    float4 color = float4(tex.Sample(smp, input.uv));
    float l = (color.r + color.g + color.b) / 3;
    return float4(l, l, l, 1);

}

float4 sepia(VSOutput input,Texture2D<float4> tex,float sepiaColor)
{
    float4 color = float4(tex.Sample(smp, input.uv));
    float l = (color.r + color.g + color.b) / 3;
    return float4(l + sepiaColor, l, saturate(l - sepiaColor), 1);
}

float Gaussian(float2 drawUV, float2 piclUV, float sigma)
{
    float d = distance(drawUV, piclUV);
    return exp(-(d * d) / (2 * sigma * sigma));
}

float4 GaussianBlur(VSOutput input, Texture2D<float4> tex)
{
    float totalWeght = 0, _Sigma = 0.005f, _StepWight = 0.001f;
    float4 color = float4(0, 0, 0, 0);
    float forRange = _Sigma * 2;
    for (float py = -forRange; py <= forRange; py += _StepWight)
    {
        for (float px = -forRange; px <= forRange; px += _StepWight)
        {
            float2 pickUV = input.uv + float2(px, py);
            float weght = Gaussian(input.uv, pickUV, _Sigma);
            color += tex.Sample(smp, pickUV) * weght;
            totalWeght += weght;
        }
    }
    color.rgb = color.rgb / totalWeght;
    return color;

}

float4 main(VSOutput input) : SV_TARGET
{
    switch (color.r)
    {
        case 1:
            if (fmod(input.uv.y, 0.1f) < 0.05f)
            {
                return AverageBlur(input, tex0);
            }
            return inv(input, tex1);
            break;
        case 2:
            return AverageBlur(input, tex0);
            break;
        case 3:
            return inv(input, tex0);
            break;
        case 4:
            return GaussianBlur(input, tex0);
            break;
        case 5:
            return gray(input, tex0);
            break;
        case 6:
            return sepia(input, tex0,0.024f);
            break;
        case 7:
            return float4(tex1.Sample(smp, input.uv));;
            break;
        default:
            if (fmod(input.uv.y, 0.1f) < 0.05f)
            {
                return AverageBlur(input,tex0); 
            }
            return inv(input, tex1);
            break;
    }
    return float4(tex1.Sample(smp, input.uv));
    
}