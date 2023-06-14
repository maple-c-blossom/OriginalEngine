#pragma once
#include "Depth.h"
#include "RootParameter.h"
#include "PIpelineRootSignature.h"
#include <array>

namespace MCB
{
	class PipeLineManager
	{
	private:
		std::array<PipelineRootSignature,4> objPipeline_;
		std::array<PipelineRootSignature,4> animeObjPipeline_;
		std::array<PipelineRootSignature,4> spritePipeline_;
		std::array<PipelineRootSignature,4> particlePipeline_;
		std::array<PipelineRootSignature,4> postEffectPipeline_;

	public:
		PipeLineManager(RootParameter* rootpamrams, Depth* depth);
		void SetObjPipeLine( int32_t blend = 0);
		void SetFbxPipeLine( int32_t blend = 0);
		void SetSpritePipeLine( int32_t blend = 0);
		void SetParticlePipeLine( int32_t blend = 0);
		void SetPostEffectPipeLine( int32_t blend = 0);
		PipelineRootSignature* Getpipeline( int32_t pipelineSelect, int32_t blend);
	};

}

