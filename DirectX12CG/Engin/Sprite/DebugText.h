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
		static const int maxCharCount = 0xff;//1�t��������̍ő�\����
		static const int fontWidth = 9;//�t�H���g�̉���
		static const int fontHeight = 18;//�t�H���g�̏c��
		static const int fontLineCount = 14;//�t�H���g�̂P�s������̕�����
		char buffer[maxCharCount]{};
	public:
		Sprite sprite[maxCharCount];
		int spriteIndex = 0;
		Texture* debugfont;
		Dx12* dx12;

		void Init(Texture* texture);
		void Print( float x, float y, float scale, const char* text, ... );
		void AllDraw();
	};

}

