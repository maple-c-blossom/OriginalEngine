#pragma once
#include <d3d12.h>
namespace MCB
{

	class TexSample
	{

	public:

        D3D12_STATIC_SAMPLER_DESC samplerDesc_{};

        void Init();

	};

}

