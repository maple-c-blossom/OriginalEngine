#pragma once
#include "Depth.h"
#include "RootParameter.h"
#include "PIpelineRootSignature.h"
#include <array>

namespace MCB
{
	namespace PLN
	{
		enum Pipelinename
		{
			Obj = 1,
			Sprite = 2,
			Particle = 3,
			Anime = 4,
			PostEffect = 5
		};
	}

	class PipeLineManager
	{
	private:
		std::array<PipelineRootSignature,4> objPipeline_;
		std::array<PipelineRootSignature,4> linePipeline_;
		std::array<PipelineRootSignature,4> animeObjPipeline_;
		std::array<PipelineRootSignature,4> spritePipeline_;
		std::array<PipelineRootSignature,4> particlePipeline_;
		std::array<PipelineRootSignature,4> postEffectPipeline_;
		std::array<PipelineRootSignature,4> objPipelineWire_;
		std::array<PipelineRootSignature,4> animeObjPipelineWire_;

	public:
		PipeLineManager(RootParameter* rootpamrams, Depth* depth);
		void SetObjPipeLine(bool wireFrame = false,  BlendMode blend = Alpha);
		void SetLinePipeLine(  BlendMode blend = Alpha);
		void SetFbxPipeLine(bool wireFrame = false, BlendMode blend = Alpha);
		void SetSpritePipeLine(BlendMode blend = Alpha);
		void SetParticlePipeLine(BlendMode blend = Alpha);
		void SetPostEffectPipeLine(BlendMode blend = Alpha);
		PipelineRootSignature* Getpipeline(PLN::Pipelinename pipelineSelect, BlendMode blend);
	};

}

