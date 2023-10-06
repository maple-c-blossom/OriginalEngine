#pragma once
#include "IgnoreWarning.h"
WarningIgnoreBegin
#include <d3d12.h>
WarningIgnoreEnd
namespace MCB
{

	class TexSample
	{

	public:

        D3D12_STATIC_SAMPLER_DESC samplerDesc_{};

        void Init();


	};

}

