cbuffer cbuff0 : register(b0)
{
    matrix viewproj : packoffset(c0);
    matrix cameraMat : packoffset(c4);
	matrix world : packoffset(c8);
    float4 color : packoffset(c12);
    float3 cameraPos : packoffset(c13);
    float shaderNum : packoffset(c13.w);
}

cbuffer cbuff1 : register(b1)
{
	float3 m_ambient :packoffset(c0);
	float3 m_diffuse :packoffset(c1);
	float3 m_specular :packoffset(c2);
	float m_alpha : packoffset(c2.w);

}
static const int DIRLIGHT_NUM = 3;
struct DirLight
{
	float3 lightv;
	float shininess;
	float3 lightcolor;
	uint active;
};

static const int PLIGHT_NUM = 3;
struct PLight
{
	float3 lightPos;
	float shininess;
	float3 lightColor;
    float pad;
	float3 lightAtten;
	uint active;
};

static const int SLIGHT_NUM = 3;
struct SLight
{
	float3 ligntVec;
	float shininess;
	float3 lightPos;
    float pad;
	float3 lightColor;
    float pad2;
	float3 lightAtten;
    float pad3;
	float2 lightFactorAngleCos;
	uint active;
    float pad4;
};

cbuffer cbuff2 : register(b2)
{
	float3 ambientColor;
	DirLight dirLights[DIRLIGHT_NUM];
	PLight pLights[PLIGHT_NUM];
	SLight sLights[SLIGHT_NUM];
}

struct VSOutput
{
	float4 svpos : SV_POSITION;
	float3 normal :NORMAL;
	float4 worldpos : POSITION;
	float2 uv : TEXCOORD;
};

struct GSOutput
{
	float4 svpos : SV_POSITION;
	float3 normal :NORMAL;
	float4 worldpos : POSITION;
	float2 uv :TEXCOORD;
};

struct PSOutput
{
    float4 target0 : SV_TARGET0;
    float4 target1 : SV_TARGET1;
};