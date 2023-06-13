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
		std::array<PipelineRootSignature,4> AnimeObjPipeline_;
		std::array<PipelineRootSignature,4> spritePipeline_;
		std::array<PipelineRootSignature,4> particlePipeline_;
		std::array<PipelineRootSignature,4> postEffectPipeline_;

	public:
		PipeLineManager(RootParameter* rootpamrams, Depth* depth);
		void SetObjPipeLine(const int32_t& blend = 0);
		void SetFbxPipeLine(const int32_t& blend = 0);
		void SetSpritePipeLine(const int32_t& blend = 0);
		void SetParticlePipeLine(const int32_t& blend = 0);
		void SetPostEffectPipeLine(const int32_t& blend = 0);
		PipelineRootSignature* Getpipeline(const int32_t& pipelineSelect,const int32_t& blend);
	};

}

