#include "PipeLineManager.h"

MCB::PipeLineManager::PipeLineManager(RootParameter* rootpamrams, Depth* depth)
{
	for (int32_t i = 0; i < objPipeline_.size(); i++) objPipeline_[i].Create3DObjectPipeline(*depth, *rootpamrams, i);
	for (int32_t i = 0; i < linePipeline_.size(); i++) linePipeline_[i].CreateLinePipeline(*depth, *rootpamrams, i);
	for (int32_t i = 0; i < objPipelineWire_.size(); i++) objPipelineWire_[i].Create3DObjectWirePipeline(*depth, *rootpamrams, i);
	for (int32_t i = 0; i < spritePipeline_.size(); i++) spritePipeline_[i].CreateSpritePipeline(*depth, *rootpamrams, i);
	for (int32_t i = 0; i < particlePipeline_.size(); i++) particlePipeline_[i].CreateParticlePipeline(*depth, *rootpamrams, i);
	for (int32_t i = 0; i < animeObjPipelineWire_.size(); i++) animeObjPipelineWire_[i].CreateFbxWirePipeline(*depth, *rootpamrams, i);
	for (int32_t i = 0; i < animeObjPipeline_.size(); i++) animeObjPipeline_[i].CreateFbxPipeline(*depth, *rootpamrams, i);
	for (int32_t i = 0; i < postEffectPipeline_.size(); i++) postEffectPipeline_[i].CreatePostEffectPipeline(*depth, *rootpamrams, i);
}

void MCB::PipeLineManager::SetObjPipeLine(bool wireFrame,BlendMode blend)
{
	if (blend >= objPipeline_.size())return;
	if (wireFrame) objPipelineWire_[blend].CommonBeginDraw();
	else objPipeline_[blend].CommonBeginDraw();
}

void MCB::PipeLineManager::SetLinePipeLine( BlendMode blend)
{
	if (blend >= linePipeline_.size())return;
	linePipeline_[blend].CommonBeginDraw(LINE);
}

void MCB::PipeLineManager::SetFbxPipeLine(bool wireFrame,BlendMode blend)
{
	if (blend >= animeObjPipeline_.size())return;
	if(wireFrame)animeObjPipelineWire_[blend].CommonBeginDraw(TRIANGLELIST);
	else animeObjPipeline_[blend].CommonBeginDraw(TRIANGLELIST);
}

void MCB::PipeLineManager::SetSpritePipeLine(BlendMode blend)
{
	if (blend >= spritePipeline_.size())return;
	spritePipeline_[blend].CommonBeginDraw();
}

void MCB::PipeLineManager::SetParticlePipeLine(BlendMode blend)
{
	if (blend >= particlePipeline_.size())return;
	particlePipeline_[blend].CommonBeginDraw(POINT);
}

void MCB::PipeLineManager::SetPostEffectPipeLine(BlendMode blend)
{
	if (blend >= spritePipeline_.size())return;
	postEffectPipeline_[blend].CommonBeginDraw();
}

MCB::PipelineRootSignature* MCB::PipeLineManager::Getpipeline(PLN::Pipelinename pipelineSelect, BlendMode blend)
{
	switch (pipelineSelect)
	{
	case PLN::Obj:
		return &objPipeline_[blend];
		break;
	case PLN::Sprite:
		return &spritePipeline_[blend];
		break;
	case PLN::Particle:
		return &particlePipeline_[blend];
		break;
	case PLN::Anime:
		return &animeObjPipeline_[blend];
		break;
	case  PLN::PostEffect:
		return &postEffectPipeline_[blend];
		break;
	default:
		return &objPipeline_[blend];
		break;
	}
}
