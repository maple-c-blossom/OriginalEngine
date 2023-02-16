#include "PostEffect.h"

MCB::PostEffect::PostEffect()
{
	size = { 500,500 };
	position = { 0,0,0 };
	color = { 1,1,1,1 };
	anchorPoint = { 0,0 };
	isFlipX = false;
	isFlipY = false;
	tex = nullptr;
}

void MCB::PostEffect::Draw()
{
	Sprite::SpriteDraw(*tex->texture);
}
