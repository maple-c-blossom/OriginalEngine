#include "DebugText.h"

void MCB::DebugText::Init(Texture* texture)
{

	debugfont_ = texture;
	dx12_ = Dx12::GetInstance();
}

void MCB::DebugText::Print(float x, float y, float scale, const std::string text, ...)
{
	
	va_list args;
	va_start(args, text);
	int32_t w = vsnprintf(buffer_, sMAX_CHAR_COUNT_ - 1, text.c_str(), args);
	for (int32_t i = 0; i < w; i++)
	{
		if (spriteIndex_ >= sMAX_CHAR_COUNT_) break;

		const unsigned char& character = buffer_[i];

		int32_t fontIndex = character - 32;
		if (character >= 0x7f)
		{
			fontIndex = 0;
		}

		int32_t fontIndexY = fontIndex / sFONT_LINE_COUNT;
		int32_t fontIndexX = fontIndex % sFONT_LINE_COUNT;

		sprite_[spriteIndex_].position_ = {x + sFONT_WIDTH_ * scale * i,y,0};
		sprite_[spriteIndex_].texLeftTop_ = {(float)fontIndexX * sFONT_WIDTH_,(float)fontIndexY * sFONT_HEIGHT};
		sprite_[spriteIndex_].cuttingSize_ = {sFONT_WIDTH_,sFONT_HEIGHT};
		sprite_[spriteIndex_].size_ = {sFONT_WIDTH_ * scale,sFONT_HEIGHT * scale};
		sprite_[spriteIndex_].anchorPoint_ = {0,0};

		sprite_[spriteIndex_].SpriteTransferVertexBuffer(debugfont_);

		sprite_[spriteIndex_].SpriteUpdate();


		spriteIndex_++;
	}
}

void MCB::DebugText::AllDraw()
{
	ShaderResource* descriptor = ShaderResource::GetInstance();
	for (int32_t i = 0; i < spriteIndex_; i++)
	{
		//SRVヒープの先頭アドレスを取得
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor->srvHeap_->GetGPUDescriptorHandleForHeapStart();


		srvGpuHandle.ptr += debugfont_->incrementNum_ * dx12_->device_.Get()->GetDescriptorHandleIncrementSize(descriptor->srvHeapDesc_.Type);

		//SRVヒープの先頭にあるSRVをパラメータ1番に設定
		dx12_->commandList_->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

		//頂点データ
		dx12_->commandList_->IASetVertexBuffers(0, 1, &sprite_[i].vbView_);
		//定数バッファビュー(CBV)の設定コマンド
		dx12_->commandList_->SetGraphicsRootConstantBufferView(0, sprite_[i].constBuff_->GetGPUVirtualAddress());
		//描画コマンド
		dx12_->commandList_->DrawInstanced(4, 1, 0, 0);
	}
	spriteIndex_ = 0;
}

