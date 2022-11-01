cbuffer cbuff0 : register(b0)
{
    matrix viewproj;
    matrix world;
    float3 cameraPos;
}

cbuffer cbuff1 : register(b1)
{
	float3 m_ambient :packoffset(c0);
	float3 m_diffuse :packoffset(c1);
	float3 m_specular :packoffset(c2);
	float m_alpha : packoffset(c2.w);
    float4 color : packoffset(c3);
}

struct VSOutput
{
	float4 svpos : SV_POSITION;
	//float4 color : COLOR;
    //float2 uv : TEXCOORD;
};

struct GSOutput
{
	float4 svpos : SV_POSITION;
	float2 uv :TEXCOORD;
};