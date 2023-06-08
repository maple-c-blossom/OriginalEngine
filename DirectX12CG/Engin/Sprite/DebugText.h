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
		static const int32_t maxCharCount = 0xff;//1�t��������̍ő�\����
		static const int32_t fontWidth = 9;//�t�H���g�̉���
		static const int32_t fontHeight = 18;//�t�H���g�̏c��
		static const int32_t fontLineCount = 14;//�t�H���g�̂P�s������̕�����
		char buffer[maxCharCount]{};
	public:
		Sprite sprite[maxCharCount];
		int32_t spriteIndex = 0;
		Texture* debugfont = nullptr;
		Dx12* dx12 = nullptr;

		void Init(Texture* texture);
		void Print( float x, float y, float scale, const char* text, ... );
		void AllDraw();
	};

}

