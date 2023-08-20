#include "Pipeline.h"

void MCB::Pipeline::SetSampleMask()
{
	pipelineDesc_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
}

void MCB::Pipeline::SetSampleMask(size_t sampleMask)
{
	pipelineDesc_.SampleMask = static_cast<uint32_t>(sampleMask);
}

void MCB::Pipeline::SetRasterizerState(bool DepthClipEnable ,const D3D12_CULL_MODE& cullmode,const D3D12_FILL_MODE& fillmode)
{
	pipelineDesc_.RasterizerState.CullMode = cullmode;  // 背面カリング
	pipelineDesc_.RasterizerState.FillMode = fillmode; // ポリゴン内塗りつぶし
	pipelineDesc_.RasterizerState.DepthClipEnable = DepthClipEnable; // 深度クリッピングを有効に
}

void MCB::Pipeline::SetAllAddRasterizerState()
{
	pipelineDesc_.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;  // 背面カリング
	pipelineDesc_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	pipelineDesc_.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に
}

void MCB::Pipeline::SetSpriteAllAddRasterizerState()
{
	pipelineDesc_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;  // 背面カリング
	pipelineDesc_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	pipelineDesc_.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に
}

void MCB::Pipeline::SetGpipleneDesc(D3D12_SHADER_BYTECODE &byteCode, ID3DBlob* blob)
{
	byteCode.pShaderBytecode = blob->GetBufferPointer();
	byteCode.BytecodeLength = blob->GetBufferSize();

}

void MCB::Pipeline::SetGpipleneDescVS(ID3DBlob* blob)
{
	pipelineDesc_.VS.pShaderBytecode = blob->GetBufferPointer();
	pipelineDesc_.VS.BytecodeLength = blob->GetBufferSize();

}

void MCB::Pipeline::SetGpipleneDescPS(ID3DBlob* blob)
{
	pipelineDesc_.PS.pShaderBytecode = blob->GetBufferPointer();
	pipelineDesc_.PS.BytecodeLength = blob->GetBufferSize();
	pipelineDesc_.PS.BytecodeLength = blob->GetBufferSize();
}

void MCB::Pipeline::SetGpipleneDescAll(ID3DBlob* VS, ID3DBlob* PS)
{
	pipelineDesc_.VS.pShaderBytecode = VS->GetBufferPointer();
	pipelineDesc_.VS.BytecodeLength = VS->GetBufferSize();

	pipelineDesc_.PS.pShaderBytecode = PS->GetBufferPointer();
	pipelineDesc_.PS.BytecodeLength = PS->GetBufferSize();
}

void MCB::Pipeline::SetGpipleneDescAll(Shader* shader)
{
	pipelineDesc_.VS.pShaderBytecode = shader->vsBlob_->GetBufferPointer();
	pipelineDesc_.VS.BytecodeLength = shader->vsBlob_->GetBufferSize();
	if (shader->gsBlob_)
	{
		pipelineDesc_.GS.pShaderBytecode = shader->gsBlob_->GetBufferPointer();
		pipelineDesc_.GS.BytecodeLength = shader->gsBlob_->GetBufferSize();
	}
	pipelineDesc_.PS.pShaderBytecode = shader->psBlob_->GetBufferPointer();
	pipelineDesc_.PS.BytecodeLength = shader->psBlob_->GetBufferSize();

}

void MCB::Pipeline::SetSpriteGpipleneDescAll(Shader* shader)
{
	pipelineDesc_.VS.pShaderBytecode = shader->vsBlob_->GetBufferPointer();
	pipelineDesc_.VS.BytecodeLength = shader->vsBlob_->GetBufferSize();

	pipelineDesc_.PS.pShaderBytecode = shader->psBlob_->GetBufferPointer();
	pipelineDesc_.PS.BytecodeLength = shader->psBlob_->GetBufferSize();

}


void MCB::Pipeline::SetNormalBlendDesc()
{
	//共通設定
	blenddesc_.BlendEnable = true;
	blenddesc_.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc_.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc_.DestBlendAlpha = D3D12_BLEND_ZERO;
}

void MCB::Pipeline::SetNormalBlendDesc(bool blendEnable,const D3D12_BLEND_OP& blendOp,
	const D3D12_BLEND& srcBlend,const D3D12_BLEND& destBlend)
{
	blenddesc_.BlendEnable = blendEnable;
	blenddesc_.BlendOpAlpha = blendOp;
	blenddesc_.SrcBlendAlpha = srcBlend;
	blenddesc_.DestBlendAlpha = destBlend;
}

void MCB::Pipeline::SetAlphaBlend()
{
	blenddesc_.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc_.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc_.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
}

void MCB::Pipeline::SetAddBlend()
{
	blenddesc_.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc_.SrcBlend = D3D12_BLEND_ONE;
	blenddesc_.DestBlend = D3D12_BLEND_ONE;
}

void MCB::Pipeline::SetSubBlend()
{
	blenddesc_.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
	blenddesc_.SrcBlend = D3D12_BLEND_ONE;
	blenddesc_.DestBlend = D3D12_BLEND_ONE;
}

void MCB::Pipeline::SetInvBlend()
{
	blenddesc_.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc_.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
	blenddesc_.DestBlend = D3D12_BLEND_ZERO;
}

void MCB::Pipeline::SetPrimitiveTopologyType(const D3D12_PRIMITIVE_TOPOLOGY_TYPE& topologyType)
{
	pipelineDesc_.PrimitiveTopologyType = topologyType;
}

void MCB::Pipeline::SetNumRenderTargets(size_t numRenderTarget)
{
	pipelineDesc_.NumRenderTargets = static_cast<uint32_t>(numRenderTarget);
}

void MCB::Pipeline::SetRTVFormats(const DXGI_FORMAT& RTVFormat, size_t FormatNum)
{
	if (FormatNum >= 8) return;

	pipelineDesc_.RTVFormats[FormatNum] = RTVFormat;

}

void MCB::Pipeline::SetSampleDescCount(size_t count)
{
	pipelineDesc_.SampleDesc.Count = static_cast<uint32_t>( count);
}

void MCB::Pipeline::SetRootSignature(const RootSignature& rootSignature)
{
	pipelineDesc_.pRootSignature = rootSignature.rootsignature_.Get();
}

void MCB::Pipeline::CreateGraphicsPipelineState()
{
	Dx12::GetInstance()->result_ = Dx12::GetInstance()->device_->CreateGraphicsPipelineState(&pipelineDesc_, IID_PPV_ARGS(&pipelinestate_));
	assert(SUCCEEDED(Dx12::GetInstance()->result_));
}



void MCB::Pipeline::SetRenderTaegetBlendDesc(const D3D12_RENDER_TARGET_BLEND_DESC& renderTarget)
{
	blenddesc_ = renderTarget;
}

void MCB::Pipeline::SetRenderTargetWriteMask(const D3D12_COLOR_WRITE_ENABLE& writeEnable)
{
	blenddesc_.RenderTargetWriteMask = writeEnable;//標準設定
}
