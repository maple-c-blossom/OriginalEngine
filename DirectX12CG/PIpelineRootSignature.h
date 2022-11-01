#pragma once
#include "Pipeline.h"
#include "RootSignature.h"
#include "Shader.h"
#include "Depth.h"
#include "TexSample.h"
#include "RootParameter.h"

namespace MCB
{
	enum BlendMode
	{
		Alpha = 0,
		Add = 1,
		Sub = 2,
		Inv = 3
	};
	class PipelineRootSignature
	{
	public:
		Pipeline pipeline;
		RootSignature rootsignature;
		Shader shader;
		TexSample texSample;

		void Create3DObjectPipeline(Depth& depth, RootParameter& rootparam,int blendMode = Alpha );

		void CreateSpritePipeline(Depth& depth, RootParameter& rootparam, int blendMode = Alpha);

		void CreateParticlePipeline(Depth& depth, RootParameter& rootparam, int blendMode = Alpha);

		void SetBrendMode(int blendMode = Alpha);

		void CommonBeginDraw(bool toporogyTypeIsPoint = false);

	};
}

