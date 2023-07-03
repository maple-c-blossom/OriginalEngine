#include "PipeLineManager.h"

MCB::PipeLineManager::PipeLineManager(RootParameter* rootpamrams, Depth* depth)
{
	for (int32_t i = 0; i < objPipeline_.size(); i++) objPipeline_[i].Create3DObjectPipeline(*depth, *rootpamrams, i);
	for (int32_t i = 0; i < spritePipeline_.size(); i++) spritePipeline_[i].CreateSpritePipeline(*depth, *rootpamrams, i);
	for (int32_t i = 0; i < particlePipeline_.size(); i++) particlePipeline_[i].CreateParticlePipeline(*depth, *rootpamrams, i);
	for (int32_t i = 0; i < animeObjPipeline_.size(); i++) animeObjPipeline_[i].CreateFbxPipeline(*depth, *rootpamrams, i);
	for (int32_t i = 0; i < postEffectPipeline_.size(); i++) postEffectPipeline_[i].CreatePostEffectPipeline(*depth, *rootpamrams, i);
}

void MCB::PipeLineManager::SetObjPipeLine(int32_t blend)
{
	if (blend >= objPipeline_.size())return;
	objPipeline_[blend].CommonBeginDraw();
}

void MCB::PipeLineManager::SetFbxPipeLine(int32_t blend)
{
	if (blend >= animeObjPipeline_.size())return;
	animeObjPipeline_[blend].CommonBeginDraw(TRIANGLELIST);
}

void MCB::PipeLineManager::SetSpritePipeLine(int32_t blend)
{
	if (blend >= spritePipeline_.size())return;
	spritePipeline_[blend].CommonBeginDraw();
}

void MCB::PipeLineManager::SetParticlePipeLine(int32_t blend)
{
	if (blend >= particlePipeline_.size())return;
	particlePipeline_[blend].CommonBeginDraw(POINT);
}

void MCB::PipeLineManager::SetPostEffectPipeLine(int32_t blend)
{
	if (blend >= spritePipeline_.size())return;
	postEffectPipeline_[blend].CommonBeginDraw();
}

MCB::PipelineRootSignature* MCB::PipeLineManager::Getpipeline( int32_t pipelineSelect, int32_t blend)
{
	switch (pipelineSelect)
	{
	case 1:
		return &objPipeline_[blend];
		break;
	case 2:
		return &spritePipeline_[blend];
		break;
	case 3:
		return &particlePipeline_[blend];
		break;
	case 4:
		return &animeObjPipeline_[blend];
		break;
	case 5:
		return &postEffectPipeline_[blend];
		break;
	default:
		return &objPipeline_[blend];
		break;
	}
}
