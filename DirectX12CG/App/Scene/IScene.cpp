#include "IScene.h"

MCB::IScene::~IScene()
{
}

MCB::IScene::IScene()
{
}

void MCB::IScene::ImGuiDraw()
{
	imgui_.Draw();
}

bool MCB::IScene::GetIsSceneEnd()
{
	return sceneEnd_;
}

MCB::Depth* MCB::IScene::GetDepthPtr()
{
	return depth_;
}

MCB::Depth& MCB::IScene::GetDepth()
{
	return *depth_;
}

MCB::RootParameter* MCB::IScene::GetRootparamsPtr()
{
	return rootparamsPtr_;
}

MCB::PipeLineManager* MCB::IScene::GetpipelinePtr()
{
	return pipeline_;
}

MCB::PipeLineManager& MCB::IScene::Getpipeline()
{
	return *pipeline_;
}