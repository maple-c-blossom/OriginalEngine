#include "FbxShaderHeader.hlsli"

struct SkinOutput
{
    float4 pos;
    float3 normal;
};

SkinOutput ComputeSkin(VSInput input)
{
    SkinOutput output = (SkinOutput) 0;
    
    uint boneId;
    float weight;
    matrix m;
    
    boneId = input.ids.x;
    weight = input.weights.x;
    m = matSkinning[boneId];
    output.pos += weight * mul(m, input.worldpos);
    output.normal += weight * mul((float3x3) m, input.normal);
    
    boneId = input.ids.y;
    weight = input.weights.y;
    m = matSkinning[boneId];
    output.pos += weight * mul(m, input.worldpos);
    output.normal += weight * mul((float3x3) m, input.normal);
    
    
    boneId = input.ids.z;
    weight = input.weights.z;
    m = matSkinning[boneId];
    output.pos += weight * mul(m, input.worldpos);
    output.normal += weight * mul((float3x3) m, input.normal);
    
    boneId = input.ids.w;
    weight = input.weights.w;
    m = matSkinning[boneId];
    output.pos += weight * mul(m, input.worldpos);
    output.normal += weight * mul((float3x3) m, input.normal);
    return output;

}

VSOutput main(VSInput input)
{
    SkinOutput skinned = ComputeSkin(input);
    float4 wnormal = normalize(mul(world, float4(skinned.normal, 0)));
    float4 wpos = mul(world, input.worldpos);
    float4x4 wcMat = mul(cameraMat, world);

    VSOutput output;
    output.svpos = mul(mul(viewproj, wcMat), skinned.pos);
    output.worldpos = wpos;
    output.normal = wnormal.xyz;
    output.uv = input.uv;
    //output.ids = input.ids;
    //output.weights = input.weights;

    
    
    return output;
}