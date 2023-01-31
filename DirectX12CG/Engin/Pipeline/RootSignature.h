#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "RootParameter.h"
#include "TexSample.h"
#include "Shader.h"
#include "Dx12.h"
namespace MCB
{
	class RootSignature
	{
	public:
		Microsoft::WRL::ComPtr <ID3D12RootSignature> rootsignature;

		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};

		Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob = nullptr;
		
		void InitRootSignatureDesc(RootParameter& rootparams, TexSample& sample);
		void SetSerializeRootSignature(Shader& shader);
		void CreateRootSignature();
	};
}

