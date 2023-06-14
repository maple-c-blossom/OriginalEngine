#pragma once
#include <d3d12.h>
#include "RootSignature.h"
namespace MCB
{
	class Pipeline
	{
	public:


		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc_{};

		D3D12_RENDER_TARGET_BLEND_DESC& blenddesc_ = pipelineDesc_.BlendState.RenderTarget[0];

		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate_ = nullptr;

		void SetSampleMask();

		void SetSampleMask(size_t sampleMask);

		void SetRasterizerState(bool DepthClipEnable = true, 
			const D3D12_CULL_MODE& cullmode = D3D12_CULL_MODE_BACK,
			const D3D12_FILL_MODE& fillmode = D3D12_FILL_MODE_SOLID);

		void SetAllAddRasterizerState();

		void SetSpriteAllAddRasterizerState();

		void SetGpipleneDesc(D3D12_SHADER_BYTECODE &byteCode,ID3DBlob* blob);

		void SetGpipleneDescVS(ID3DBlob* blob);

		void SetGpipleneDescPS(ID3DBlob* blob);

		void SetGpipleneDescAll(ID3DBlob* VS, ID3DBlob* PS);

		void SetGpipleneDescAll(Shader* shader);

		void SetSpriteGpipleneDescAll(Shader* shader);

		void SetRenderTaegetBlendDesc(const D3D12_RENDER_TARGET_BLEND_DESC& renderTarget);

		void SetRenderTargetWriteMask(const D3D12_COLOR_WRITE_ENABLE& writeEnable = D3D12_COLOR_WRITE_ENABLE_ALL);



		void SetNormalBlendDesc();

		void SetNormalBlendDesc(bool blendEnable, const D3D12_BLEND_OP& blendOp, 
			const D3D12_BLEND& srcBlend, const D3D12_BLEND& destBlend);

		void SetAlphaBlend();

		void SetAddBlend();

		void SetSubBlend();

		void SetInvBlend();

		void SetPrimitiveTopologyType(
			const D3D12_PRIMITIVE_TOPOLOGY_TYPE& topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);

		void SetNumRenderTargets(size_t numRenderTarget = 1);

		void SetRTVFormats(const DXGI_FORMAT& RTVFormat = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, size_t FormatNum = 0);

		void SetSampleDescCount(size_t count = 1);

		void SetRootSignature(const RootSignature& rootSignature);

		void CreateGraphicsPipelineState();

	};
}

