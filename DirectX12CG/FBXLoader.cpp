#include "FBXLoader.h"
#include "FBXModel.h"

using namespace MCB;

const std::string AssimpLoader::baseDirectory = "Resources\\";

void AssimpLoader::DeleteInstance()
{
	
	GetInstance()->Finalize();
	delete GetInstance();
}

AssimpLoader* AssimpLoader::GetInstance()
{
	static AssimpLoader* instance = new AssimpLoader;
	return instance;
}

AssimpLoader* AssimpLoader::GetInitInstance()
{
	static AssimpLoader* instance = GetInstance();
	instance->Initialize();
	return instance;
}

void AssimpLoader::Initialize()
{
;


}

void MCB::AssimpLoader::Finalize()
{

}



