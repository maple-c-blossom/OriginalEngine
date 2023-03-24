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
		const float clearColor[4] = { 0.25f,0.5f,0.1f,1.0f };
	public:
		PostEffect();
		TextureCell* tex;
		void Init();
		void PreDraw();
		void Draw();
		void PostDraw();
	};
}

