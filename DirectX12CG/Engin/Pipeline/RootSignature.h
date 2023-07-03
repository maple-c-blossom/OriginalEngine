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
		Microsoft::WRL::ComPtr <ID3D12RootSignature> rootsignature_;

		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc_{};

		Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob_ = nullptr;
		
		void InitRootSignatureDesc(RootParameter& rootparams, TexSample& sample);
		void SetSerializeRootSignature(Shader& shader);
		void CreateRootSignature();
	};
}

