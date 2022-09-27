#include "SceneManager.h"
#include "Scene.h"

MCB::SceneManager::SceneManager(RootParameter* root, Depth* depth, PipelineRootSignature* pipeline, PipelineRootSignature* pipeline1)
{
	this->root = root;
	this->pipeline = pipeline;
	this->pipeline1 = pipeline1;
	this->depth = depth;
	scene = new Scene(this->root, this->depth, this->pipeline, this->pipeline1);
}

MCB::SceneManager::~SceneManager()
{
	delete scene;
}

void MCB::SceneManager::Initialize()
{
	scene->Initialize();
}

void MCB::SceneManager::Update()
{
	scene->Update();
}

void MCB::SceneManager::Draw()
{
	scene->Draw();
}
