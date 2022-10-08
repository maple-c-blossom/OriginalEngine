#include "FBXLoader.h"

using namespace MCB;

const std::string FBXLoader::baseDirectory = "Resources\\";

void FBXLoader::DeleteInstance()
{
	delete GetInstance();
}

FBXLoader* FBXLoader::GetInstance()
{
	static FBXLoader* instance = new FBXLoader;
	return instance;
}

FBXLoader* FBXLoader::GetInitInstance()
{
	static FBXLoader* instance = GetInstance();
	instance->Initialize();
	return instance;
}

void FBXLoader::Initialize()
{
	assert(manager == nullptr);
	this->device = Dx12::GetInstance()->device.Get();
	manager = FbxManager::Create();

	FbxIOSettings* ios = FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(ios);
	
	fbxImporter = FbxImporter::Create(manager, "");


}

void MCB::FBXLoader::Finalize()
{
	fbxImporter->Destroy();
	manager->Destroy();
}

void MCB::FBXLoader::LoadModelFromFile(const string& modelName)
{
	const string directoryPath = baseDirectory + modelName + "\\";
	const string fileName = modelName + ".fbx";
	const string fullPath = directoryPath + fileName;
	if (!fbxImporter->Initialize(fullPath.c_str()), -1, manager->GetIOSettings()) 
	{
		assert(1);
	}

	FbxScene* fbxScene = FbxScene::Create(manager, "fbxScene");

	fbxImporter->Import(fbxScene);
}
