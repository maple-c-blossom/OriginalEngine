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
		std::array<PipelineRootSignature,4> objPipeline;
		std::array<PipelineRootSignature,4> fbxPipeline;
		std::array<PipelineRootSignature,4> spritePipeline;
		std::array<PipelineRootSignature,4> particlePipeline;
		std::array<PipelineRootSignature,4> postEffectPipeline;

	public:
		PipeLineManager(RootParameter* rootpamrams, Depth* depth);
		void SetObjPipeLine(int32_t blend = 0);
		void SetFbxPipeLine(int32_t blend = 0);
		void SetSpritePipeLine(int32_t blend = 0);
		void SetParticlePipeLine(int32_t blend = 0);
		void SetPostEffectPipeLine(int32_t blend = 0);
		PipelineRootSignature* Getpipeline(const int32_t& pipelineSelect,const int32_t& blend);
	};

}

