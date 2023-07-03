#pragma once
#include <d3d12.h>
#include <vector>
#include "Descriptor.h"

namespace MCB
{
	class RootParameter
	{
		public:
			std::vector<D3D12_ROOT_PARAMETER> rootparams_;
		~RootParameter();

		void SetRootParam(const D3D12_ROOT_PARAMETER_TYPE& paramType,  int32_t ShaderRegister,  int32_t RegisterSpace,
			const D3D12_SHADER_VISIBILITY& shaderVisibility,  int32_t NumDescriptorRanges,
			size_t descriptorIndex = 0);

		void SetRootParam(const D3D12_ROOT_PARAMETER_TYPE& paramType, 
			 int32_t ShaderRegister = 0, size_t descriptorIndex = 0);

	};

}

