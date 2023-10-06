#pragma once
#include "Sprite.h"
#include "TextureManager.h"
#include "IgnoreWarning.h"
WarningIgnoreBegin
#include <array>
#include <memory>
WarningIgnoreEnd
namespace MCB
{

	class PostEffect :public Sprite
	{
	private:
		//std::array<Microsoft::WRL::ComPtr<ID3D12Resource>,2> texBuff;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapSRV_;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapDSV_;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapRTV_;
		Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff_;
		const float CLEAR_COLOR_[4] = { 0.25f,0.5f,0.1f,1.0f };
	public:
		PostEffect();
		~PostEffect();
		PostEffect& operator=(const PostEffect&) = delete;
		std::array < TextureCell*,2> tex_;
		void Init();
		void PreDraw();
		void Draw();
		void PostDraw();
	};
}

