#pragma once
#include "IgnoreWarning.h"
WarningIgnoreBegin
#include <string>
WarningIgnoreEnd
#include "Sprite.h"
#include "Dx12.h"
#include "DxWindow.h"
#include "Descriptor.h"

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
		Byte1 pad;
	public:
		Sprite sprite_[sMAX_CHAR_COUNT_];
		int32_t spriteIndex_ = 0;
		Byte4 pad1;
		Texture* debugfont_ = nullptr;
		Dx12* dx12_ = nullptr;
		int32_t pad2;
		Byte4 pad3;
		void Init(Texture* texture);
		void Print( float x,  float y,  float scale, const char* text, ... );
		void AllDraw();
	};

}

