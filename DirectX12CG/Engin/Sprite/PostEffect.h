#pragma once
#include "Sprite.h"
#include <array>
#include <memory>
#include "TextureManager.h"
namespace MCB
{
	class PostEffect :public Sprite
	{
	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapSRV;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapDSV;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapRTV;
		Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;

	public:
		PostEffect();
		TextureCell* tex;
		void Init();
		void Draw();
	};
}

