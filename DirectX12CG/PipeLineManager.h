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
		std::array<PipelineRootSignature,4> spritePipeline;
		std::array<PipelineRootSignature,4> particlePipeline;

	public:
		PipeLineManager(RootParameter* rootpamrams, Depth* depth);
		void SetObjPipeLine(int blend = 0);
		void SetSpritePipeLine(int blend = 0);
		void SetParticlePipeLine(int blend = 0);
		PipelineRootSignature* Getpipeline(int pipelineSelect, int blend);
	};

}

