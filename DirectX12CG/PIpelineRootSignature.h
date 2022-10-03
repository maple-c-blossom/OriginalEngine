#pragma once
#include "Pipeline.h"
#include "RootSignature.h"
#include "Shader.h"
#include "Depth.h"
#include "TexSample.h"
#include "RootParameter.h"

namespace MCB
{

	class PipelineRootSignature
	{
	public:
		Pipeline pipeline;
		RootSignature rootsignature;
		Shader shader;
		TexSample texSample;

		PipelineRootSignature Create3DObjectPipeline(Depth& depth, RootParameter& rootparam);

		PipelineRootSignature CreateSpritePipeline(Depth& depth, RootParameter& rootparam);

		PipelineRootSignature CreateParticlePipeline(Depth& depth, RootParameter& rootparam);

		void CommonBeginDraw(bool toporogyTypeIsPoint = false);

	};
}

