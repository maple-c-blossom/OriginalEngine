#include "LevelLoader.h"
#include "MeshCollider.h"
#include "PlaneCollider.h"
#include "ModelManager.h"
#include "Util.h"
using namespace MCB;
using namespace std;
using namespace nlohmann;

const std::string LevelLoader::sBASE_FILE_DIR_ = "Resources/levels/";
const std::string LevelLoader::sEXTEND_ = ".json";

void LevelLoader::RecursiveAnalysis(LevelData* levelData, nlohmann::json objJson, ICamera* camera)
{

	assert(objJson.contains("Type"));
	string type = objJson["Type"].get<string>();
	if (type.compare("MESH") == 0)
	{
		std::unique_ptr<LevelData::ObjectData> objData = make_unique< LevelData::ObjectData>();
		objData->obj.Init();
		if (objJson.contains("file_name"))
		{
			objData->fileName = objJson["file_name"];
			objData->obj.model_ = ModelManager::GetModel(objData->fileName);
		}
		json& transform = objJson["transform"];
		objData->obj.position_.x = static_cast<float>(transform["translation"][0]);
		objData->obj.position_.y = static_cast<float>(transform["translation"][2]);
		objData->obj.position_.z = static_cast<float>(transform["translation"][1]);
		objData->obj.rotation_.x = ConvertRadius(static_cast<float>(transform["rotation"][1]));
		objData->obj.rotation_.y = ConvertRadius (-static_cast<float>(transform["rotation"][2]));
		objData->obj.rotation_.z = ConvertRadius (-static_cast<float>(transform["rotation"][0]));
		objData->obj.scale_.x = static_cast<float>(transform["scaling"][0]);
		objData->obj.scale_.y = static_cast<float>(transform["scaling"][1]);
		objData->obj.scale_.z = static_cast<float>(transform["scaling"][2]);

		if (!(objData->fileName == "skydome"))
		{
			objData->obj.SetCollider(make_unique<MeshCollider>(objData->obj.model_));
		}
		objData->obj.camera_ = camera;
		objData->obj.nameId_ = objData->fileName;
		levelData->objects.emplace_back(move(objData));
	}
	if (objJson.contains("children"))
	{
		for (json& child : objJson["children"]) RecursiveAnalysis(levelData, child,camera);
	}
}

std::unique_ptr<LevelLoader::LevelData> LevelLoader::Load(const std::string& fileName, ICamera* camera)
{
	const string path = sBASE_FILE_DIR_ + fileName + sEXTEND_;
	ifstream file;
	file.open(path);
	if (file.fail())
	{
		assert(0 && "レベルデータ読み込み不良");
	}

	json deserialize;

	file >> deserialize;
	assert(deserialize.is_object() && "レベルデータに異常があります");
	assert(deserialize.contains("name") && "レベルデータに異常があります");
	assert(deserialize["name"].is_string() && "レベルデータに異常があります");
	string name = deserialize["name"].get<string>();
	assert(name.compare("scene") == 0 && "レベルデータが正しくありません");
	unique_ptr<LevelData> levelData = make_unique<LevelData>();
	levelData->levelFileName = fileName;
	levelData->camera = camera;
	for (json& obj : deserialize["objects"])
	{
		RecursiveAnalysis(levelData.get(), obj,camera);
	}
	file.close();
	return levelData;
}





void MCB::LevelLoader::LevelData::Update()
{
	for (auto& itr : objects)
	{
		itr->obj.UniqueUpdate();
	}
}

void MCB::LevelLoader::LevelData::UpdateMatrix()
{
	for (auto& itr : objects)
	{
		itr->obj.Update();
	}
}

void MCB::LevelLoader::LevelData::Draw()
{
	for (auto& itr : objects)
	{
		if (itr->obj.animationModel_)itr->obj.AnimationDraw();
		else itr->obj.Draw();
	}
}

std::unique_ptr<LevelLoader::LevelData> MCB::LevelLoader::LevelData::ReLoad()
{
	return Load(levelFileName,camera);
}
