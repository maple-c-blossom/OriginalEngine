#pragma once
#include <d3d12.h>
#include <vector>
#include "Descriptor.h"

namespace MCB
{
	class RootParameter
	{
		public:
			std::vector<D3D12_ROOT_PARAMETER> rootparams;
		~RootParameter();

		void SetRootParam(D3D12_ROOT_PARAMETER_TYPE paramType, int ShaderRegister, int RegisterSpace,
			D3D12_SHADER_VISIBILITY shaderVisibility, int NumDescriptorRanges, unsigned int descriptorIndex = 0);

		void SetRootParam(D3D12_ROOT_PARAMETER_TYPE paramType, int ShaderRegister = 0, unsigned int descriptorIndex = 0);

	};

}

