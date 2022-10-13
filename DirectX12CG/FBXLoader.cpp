#include "FBXLoader.h"
#include "FBXModel.h"

using namespace MCB;

const std::string FBXLoader::baseDirectory = "Resources\\";

void FBXLoader::DeleteInstance()
{
	
	GetInstance()->Finalize();
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

	std::unique_ptr<FBXModel> model = std::make_unique<FBXModel>();
	model->name = modelName;
	ParseNodeRecursive(model.get(), fbxScene->GetRootNode());
	fbxScene->Destroy();
	
}
