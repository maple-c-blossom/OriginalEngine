#pragma once
#include "Pipeline.h"
#include "RootSignature.h"
#include "Shader.h"
#include "Depth.h"
#include "TexSample.h"
#include "RootParameter.h"

namespace MCB
{
	static enum BlendMode
	{
		Alpha,
		Add,
		Sub,
		Inv
	};
	class PipelineRootSignature
	{
	public:
		Pipeline pipeline;
		RootSignature rootsignature;
		Shader shader;
		TexSample texSample;

		PipelineRootSignature Create3DObjectPipeline(Depth& depth, RootParameter& rootparam,int blendMode = Alpha );

		PipelineRootSignature CreateSpritePipeline(Depth& depth, RootParameter& rootparam, int blendMode = Alpha);

		PipelineRootSignature CreateParticlePipeline(Depth& depth, RootParameter& rootparam, int blendMode = Alpha);

		void SetBrendMode(int blendMode = Alpha);

		void CommonBeginDraw(bool toporogyTypeIsPoint = false);

	};
}

