#pragma once
#include "Sprite.h"
#include "Dx12.h"
#include "DxWindow.h"
#include "Descriptor.h"
#include <string>

namespace MCB
{
	class DebugText
	{
	private:
		static const int32_t sMAX_CHAR_COUNT_ = 0xff;//1フレ当たりの最大表示数
		static const int32_t sFONT_WIDTH_ = 9;//フォントの横幅
		static const int32_t sFONT_HEIGHT = 18;//フォントの縦幅
		static const int32_t sFONT_LINE_COUNT = 14;//フォントの１行当たりの文字数
		char buffer_[sMAX_CHAR_COUNT_]{};
	public:
		Sprite sprite_[sMAX_CHAR_COUNT_];
		int32_t spriteIndex_ = 0;
		Texture* debugfont_ = nullptr;
		Dx12* dx12_ = nullptr;

		void Init(Texture* texture);
		void Print( float x,  float y,  float scale, const std::string text, ... );
		void AllDraw();
	};

}

