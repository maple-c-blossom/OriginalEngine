#include "LevelLoader.h"
using namespace MCB;
using namespace std;
using namespace nlohmann;

const std::string LevelLoader::sBASE_FILE_DIR_ = "Resources/levels/";
const std::string LevelLoader::sEXTEND_ = ".json";

void LevelLoader::RecursiveAnalysis(LevelData* levelData, nlohmann::json objJson)
{

	assert(objJson.contains("Type"));
	string type = objJson["Type"].get<string>();
	if (type.compare("MESH") == 0)
	{
		levelData->objects.emplace_back(LevelData::ObjectData{});
		LevelData::ObjectData& objData = levelData->objects.back();
		if (objJson.contains("file_name"))
		{
			objData.fileName = objJson["file_name"];
		}
		json& transform = objJson["transform"];
		objData.obj.position_.x = static_cast<float>(transform["translation"][1]);
		objData.obj.position_.y = static_cast<float>(transform["translation"][2]);
		objData.obj.position_.z = -static_cast<float>(transform["translation"][0]);
		objData.obj.rotation_.x = static_cast<float>(transform["rotation"][1]);
		objData.obj.rotation_.y = -static_cast<float>(transform["rotation"][2]);
		objData.obj.rotation_.z = -static_cast<float>(transform["rotation"][0]);
		objData.obj.scale_.x = static_cast<float>(transform["scaling"][1]);
		objData.obj.scale_.y = static_cast<float>(transform["scaling"][2]);
		objData.obj.scale_.z = static_cast<float>(transform["scaling"][0]);
	}
	if (objJson.contains("children"))
	{
		for (json& child : objJson["children"]) RecursiveAnalysis(levelData, child);
	}
}

std::unique_ptr<LevelLoader::LevelData> LevelLoader::Load(const std::string& fileName)
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
	for (json& obj : deserialize["objects"])
	{
		RecursiveAnalysis(levelData.get(), obj);
	}
	file.close();
	return levelData;
}