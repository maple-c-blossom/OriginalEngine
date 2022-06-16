#pragma once
#include "Dx12.h"
#include "Depth.h"
#include "Descriptor.h"
#include "PIpelineRootSignature.h"

namespace MCB
{
	class Draw
	{
	public:

		static void AfterDraw(Dx12 &dx12,Depth depth, ShaderResource srv,PipelineRootSignature pipeline);

	};

}

