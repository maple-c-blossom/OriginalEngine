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

	enum Toporogy
	{
		TRIANGLESTRIP,
		TRIANGLELIST,
		POINT,
	};
	class PipelineRootSignature
	{
	public:
		Pipeline pipeline_;
		RootSignature rootsignature_;
		Shader shader_;
		TexSample texSample_;

		void Create3DObjectPipeline(Depth& depth, RootParameter& rootparam, int32_t blendMode = Alpha );

		void CreateSpritePipeline(Depth& depth, RootParameter& rootparam,  int32_t blendMode = Alpha);

		void CreateParticlePipeline(Depth& depth, RootParameter& rootparam,  int32_t blendMode = Alpha);

		void CreateFbxPipeline(Depth& depth, RootParameter& rootparam,  int32_t blendMode = Alpha);

		void CreatePostEffectPipeline(Depth& depth, RootParameter& rootparam,  int32_t blendMode);

		void SetBrendMode( int32_t blendMode = Alpha);

		void CommonBeginDraw( int32_t toporogyType = TRIANGLESTRIP);

	};
}

