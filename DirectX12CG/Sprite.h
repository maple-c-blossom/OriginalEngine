#pragma once
#include "MCBFloat.h"
#include <d3d12.h>
#include <wrl.h>

namespace MCB
{
	class Sprite
	{
		typedef struct SpriteVertex
		{
			Float3 pos;
			Float2 uv;
		};

		typedef struct PipelineSet
		{
			Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
			Microsoft::WRL::ComPtr <ID3D12RootSignature> rootsignature;
		};


		//PipelineSet 

	};

}

