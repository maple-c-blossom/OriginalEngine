#include "texSample.h"

void MCB::TexSample::Init()
{
    samplerDesc_.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc_.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc_.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc_.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
    samplerDesc_.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc_.MaxLOD = D3D12_FLOAT32_MAX;
    samplerDesc_.MinLOD = 0.0f;
    samplerDesc_.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    samplerDesc_.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
}
