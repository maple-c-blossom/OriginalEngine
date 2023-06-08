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
		static const int32_t maxCharCount = 0xff;//1フレ当たりの最大表示数
		static const int32_t fontWidth = 9;//フォントの横幅
		static const int32_t fontHeight = 18;//フォントの縦幅
		static const int32_t fontLineCount = 14;//フォントの１行当たりの文字数
		char buffer[maxCharCount]{};
	public:
		Sprite sprite[maxCharCount];
		int32_t spriteIndex = 0;
		Texture* debugfont = nullptr;
		Dx12* dx12 = nullptr;

		void Init(Texture* texture);
		void Print(const float& x, const float& y, const float& scale, const char* text, ... );
		void AllDraw();
	};

}

