#include "PipeLineManager.h"

MCB::PipeLineManager::PipeLineManager(RootParameter* rootpamrams, Depth* depth)
{
	for (int i = 0; i < objPipeline.size(); i++) objPipeline[i].Create3DObjectPipeline(*depth, *rootpamrams, i);
	for (int i = 0; i < spritePipeline.size(); i++) spritePipeline[i].CreateSpritePipeline(*depth, *rootpamrams, i);
	for (int i = 0; i < particlePipeline.size(); i++) particlePipeline[i].CreateParticlePipeline(*depth, *rootpamrams, i);
	for (int i = 0; i < fbxPipeline.size(); i++) fbxPipeline[i].CreateFbxPipeline(*depth, *rootpamrams, i);
	for (int i = 0; i < fbxPipeline.size(); i++) postEffectPipeline[i].CreatePostEffectPipeline(*depth, *rootpamrams, i);
}

void MCB::PipeLineManager::SetObjPipeLine(int blend)
{
	if (blend >= objPipeline.size())return;
	objPipeline[blend].CommonBeginDraw();
}

void MCB::PipeLineManager::SetFbxPipeLine(int blend)
{
	if (blend >= fbxPipeline.size())return;
	fbxPipeline[blend].CommonBeginDraw(TRIANGLELIST);
}

void MCB::PipeLineManager::SetSpritePipeLine(int blend)
{
	if (blend >= spritePipeline.size())return;
	spritePipeline[blend].CommonBeginDraw();
}

void MCB::PipeLineManager::SetParticlePipeLine(int blend)
{
	if (blend >= particlePipeline.size())return;
	particlePipeline[blend].CommonBeginDraw(POINT);
}

void MCB::PipeLineManager::SetPostEffectPipeLine(int blend)
{
	if (blend >= spritePipeline.size())return;
	spritePipeline[blend].CommonBeginDraw();
}

MCB::PipelineRootSignature* MCB::PipeLineManager::Getpipeline(int pipelineSelect, int blend)
{
	switch (pipelineSelect)
	{
	case 1:
		return &objPipeline[blend];
		break;
	case 2:
		return &spritePipeline[blend];
		break;
	case 3:
		return &particlePipeline[blend];
		break;
	case 4:
		return &fbxPipeline[blend];
		break;
	case 5:
		return &postEffectPipeline[blend];
		break;
	default:
		return &objPipeline[blend];
		break;
	}
}
