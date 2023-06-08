#include "DebugText.h"

void MCB::DebugText::Init(Texture* texture)
{

	debugfont = texture;
	dx12 = Dx12::GetInstance();
}

void MCB::DebugText::Print(float x, float y, float scale, const char* text, ...)
{
	
	va_list args;
	va_start(args, text);
	int32_t w = vsnprintf(buffer, maxCharCount - 1, text, args);
	for (int32_t i = 0; i < w; i++)
	{
		if (spriteIndex >= maxCharCount) break;

		const unsigned char& character = buffer[i];

		int32_t fontIndex = character - 32;
		if (character >= 0x7f)
		{
			fontIndex = 0;
		}

		int32_t fontIndexY = fontIndex / fontLineCount;
		int32_t fontIndexX = fontIndex % fontLineCount;

		sprite[spriteIndex].position = {x + fontWidth * scale * i,y,0};
		sprite[spriteIndex].texLeftTop = {(float)fontIndexX * fontWidth,(float)fontIndexY * fontHeight};
		sprite[spriteIndex].cuttingSize = {fontWidth,fontHeight};
		sprite[spriteIndex].size = {fontWidth * scale,fontHeight * scale};
		sprite[spriteIndex].anchorPoint = {0,0};

		sprite[spriteIndex].SpriteTransferVertexBuffer(debugfont);

		sprite[spriteIndex].SpriteUpdate();


		spriteIndex++;
	}
}

void MCB::DebugText::AllDraw()
{
	ShaderResource* descriptor = ShaderResource::GetInstance();
	for (int32_t i = 0; i < spriteIndex; i++)
	{
		//SRV�q�[�v�̐擪�A�h���X���擾
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor->srvHeap->GetGPUDescriptorHandleForHeapStart();


		srvGpuHandle.ptr += debugfont->incrementNum * dx12->device.Get()->GetDescriptorHandleIncrementSize(descriptor->srvHeapDesc.Type);

		//SRV�q�[�v�̐擪�ɂ���SRV���p�����[�^1�Ԃɐݒ�
		dx12->commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

		//���_�f�[�^
		dx12->commandList->IASetVertexBuffers(0, 1, &sprite[i].vbView);
		//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
		dx12->commandList->SetGraphicsRootConstantBufferView(0, sprite[i].constBuff->GetGPUVirtualAddress());
		//�`��R�}���h
		dx12->commandList->DrawInstanced(4, 1, 0, 0);
	}
	spriteIndex = 0;
}

