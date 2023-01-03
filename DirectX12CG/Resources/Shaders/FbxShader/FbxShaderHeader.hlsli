cbuffer cbuff0 : register(b0)
{
    matrix viewproj;
    matrix world;
    float3 cameraPos;
}

cbuffer cbuff1 : register(b1)
{
    float3 m_ambient : packoffset(c0);
    float3 m_diffuse : packoffset(c1);
    float3 m_specular : packoffset(c2);
    float m_alpha : packoffset(c2.w);
    float4 color : packoffset(c3);
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
    float3 lightAtten;
    uint active;
};

static const int SLIGHT_NUM = 3;
struct SLight
{
    float3 ligntVec;
    float shininess;
    float3 lightPos;
    float3 lightColor;
    float3 lightAtten;
    float2 lightFactorAngleCos;
    uint active;
};

cbuffer cbuff2 : register(b2)
{
    float3 ambientColor;
    DirLight dirLights[DIRLIGHT_NUM];
    PLight pLights[PLIGHT_NUM];
    SLight sLights[SLIGHT_NUM];
}
static const int MAX_SKINNING = 128; //1���_���󂯂�{�[���̐�(�ő�l)

cbuffer cbuff3 : register(b3)
{
    matrix matSkinning[MAX_SKINNING];

}



struct VSOutput
{
    float3 normal : NORMAL;
    float4 worldpos : POSITION;
    float2 uv : TEXCOORD;
    //uint4 ids : BONEID;
    //float4 weights : BONEWEIGHT;
    float4 svpos : SV_POSITION;
};

struct VSInput
{
    float3 normal : NORMAL;
    float4 worldpos : POSITION;
    float2 uv : TEXCOORD;
    uint4 ids : BONEID;
    float4 weights : BONEWEIGHT;
};

struct GSOutput
{
    float3 normal : NORMAL;
    float4 worldpos : POSITION;
    float2 uv : TEXCOORD;
    //uint4 ids : BONEID;
    //float4 weights : BONEWEIGHT;
    float4 svpos : SV_POSITION;
};