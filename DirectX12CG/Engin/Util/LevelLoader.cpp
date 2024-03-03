#include "LevelLoader.h"
#include "MeshCollider.h"
#include "PlaneCollider.h"
#include "ModelManager.h"
#include "Util.h"
#include "CheckPoint.h"
#include "Goal.h"
#include "Player.h"
#include "RotationBlock.h"
#include "MoveBlock.h"
#include "JumpPad.h"
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
		
		if (objJson.contains("tag"))
		{
			objData->tag = objJson["tag"];
			if (objData->tag == "checkPoint")
			{
				objData->obj = std::make_unique<CheckPoint>();
			}
			else if (objData->tag == "goal")
			{
				unique_ptr<Goal> temp = std::make_unique<Goal>();
				temp->popModel_ = ModelManager::GetModel("star");
				temp->SetPlayer(levelData->GetObjectPtr("player"));
				objData->obj = move(temp);
			}
			else if (objData->tag == "player")
			{
				objData->obj = std::make_unique<Player>();
			}
			else if ( objData->tag == "Rblock" )
			{
				unique_ptr<RotationBlock> temp = std::make_unique<RotationBlock>();
				json& var = objJson[ "variable" ];
				temp->rotateSpeed_.vec_.x_ = static_cast< float >( var[ "RotationSpeed" ][ 0 ] );
				temp->rotateSpeed_.vec_.y_ = static_cast< float >( var[ "RotationSpeed" ][ 1 ] );
				temp->rotateSpeed_.vec_.z_ = static_cast< float >( var[ "RotationSpeed" ][ 2 ] );
				objData->obj = std::move(temp);
			}
			else if ( objData->tag == "MoveBlock" )
			{
				unique_ptr<MoveBlock> temp = std::make_unique<MoveBlock>();
				json& var = objJson[ "variable" ];
				temp->moveVec.vec_.x_ = static_cast< float >( var[ "MoveVec" ][ 0 ] );
				temp->moveVec.vec_.y_ = static_cast< float >( var[ "MoveVec" ][ 1 ] );
				temp->moveVec.vec_.z_ = static_cast< float >( var[ "MoveVec" ][ 2 ] );
				temp->maxMoveLength = static_cast< float >( var[ "MoveMax" ]);
				temp->speed = static_cast< float >( var[ "MoveSpeed" ]);
				temp->isSetMove = false;
				objData->obj = std::move(temp);
			}
			else if ( objData->tag == "MoveBlockUP" )
			{
				unique_ptr<MoveBlock> temp = std::make_unique<MoveBlock>();
				json& var = objJson[ "variable" ];
				temp->moveVec.vec_.x_ = static_cast< float >( var[ "MoveVec" ][ 0 ] );
				temp->moveVec.vec_.y_ = static_cast< float >( var[ "MoveVec" ][ 1 ] );
				temp->moveVec.vec_.z_ = static_cast< float >( var[ "MoveVec" ][ 2 ] );
				temp->maxMoveLength = static_cast< float >( var[ "MoveMax" ] );
				temp->speed = static_cast< float >( var[ "MoveSpeed" ] );
				temp->isSetMove = true;
				objData->obj = std::move(temp);
			}
			else if ( objData->tag == "JumpPad" )
			{
				unique_ptr<JumpPad> temp = std::make_unique<JumpPad>();
				json& var = objJson[ "variable" ];
				temp->jumpVec.vec_.x_ = static_cast< float >( var[ "MoveVec" ][ 0 ] );
				temp->jumpVec.vec_.y_ = static_cast< float >( var[ "MoveVec" ][ 1 ] );
				temp->jumpVec.vec_.z_ = static_cast< float >( var[ "MoveVec" ][ 2 ] );
				objData->obj = std::move(temp);
			}
			else
			{
				objData->obj = std::make_unique<Object3d>();
			}
		}
		else
		{
			objData->tag = "NormalObject";
			objData->obj = std::make_unique<Object3d>();
		}
		if (objJson.contains("modelType"))
		{
			objData->modelType = objJson["modelType"];
		}

		if ( objJson.contains("variable") )
		{
			json& var = objJson[ "variable" ];
			auto subjectIdIter = var.find("Color");
			if ( subjectIdIter != var.end() )
			{
				objData->obj->color_.x_ = static_cast< float >( var[ "Color" ][ 0 ] );
				objData->obj->color_.y_ = static_cast< float >( var[ "Color" ][ 1 ] );
				objData->obj->color_.z_ = static_cast< float >( var[ "Color" ][ 2 ] );
			}
			else
			{
				objData->obj->color_ = { 1.f,1.f,1.f,1.f };
			}
		}


		if (objJson.contains("file_name"))
		{
			objData->fileName = objJson["file_name"];
			if (objData->modelType == "Animation")
			{
				objData->obj->animationModel_ = ModelManager::GetModel(objData->fileName, objData->obj->animationModel_);
			}
			else
			{
				objData->obj->model_ = ModelManager::GetModel(objData->fileName);
			}
		}
		json& transform = objJson["transform"];
		objData->obj->position_.x = static_cast<float>(transform["translation"][0]);
		objData->obj->position_.y = static_cast<float>(transform["translation"][2]);
		objData->obj->position_.z = static_cast<float>(transform["translation"][1]);
		objData->obj->rotation_.x = ConvertRadius(static_cast<float>(transform["rotation"][0] - 90));
		objData->obj->rotation_.y = ConvertRadius (-static_cast<float>(transform["rotation"][2]));
		objData->obj->rotation_.z = ConvertRadius (-static_cast<float>(transform["rotation"][1]));
		objData->obj->scale_.x = static_cast<float>(transform["scaling"][0]);
		objData->obj->scale_.y = static_cast<float>(transform["scaling"][1]);
		objData->obj->scale_.z = static_cast<float>(transform["scaling"][2]);

		if ((objData->tag == "NormalObject"))
		{
			objData->obj->SetCollider(make_unique<MeshCollider>(objData->obj->model_));
		}
		else if ((objData->tag == "block") || ( objData->tag == "Rblock" ) ||
			( objData->tag == "JumpPad" ) ||(objData->tag == "MoveBlock" ) ||
			(objData->tag == "MoveBlockUP" ) || (objData->tag == "checkPoint" ) )
		{
			objData->obj->SetCollider(make_unique<MeshCollider>(objData->obj->model_));
			objData->obj->GetCollider()->SetAttribute(ATTRIBUTE_LANDSHAPE);
		}


		objData->obj->Init();

		objData->obj->camera_ = camera;
		objData->obj->nameId_ = objData->tag;
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





Object3d* MCB::LevelLoader::LevelData::GetObjectPtr(std::string name)
{
	for (auto& obj : objects)
	{
		if (obj->tag == name)
		{
			return obj->obj.get();
		}
	}
	return nullptr;
}

void MCB::LevelLoader::LevelData::Update(bool start)
{
	for (auto& itr : objects)
	{
		if(start)itr->obj->UniqueUpdate();
	}
}

void MCB::LevelLoader::LevelData::UpdateMatrix()
{
	for (auto& itr : objects)
	{
		if (itr->obj->animationModel_)itr->obj->AnimationUpdate();
		else itr->obj->Update();
	}
}

void MCB::LevelLoader::LevelData::Draw(PipeLineManager* pipeline, bool wireFrame)
{
	static_cast< void >( wireFrame );
	static_cast< void >( pipeline );
	for (auto& itr : objects)
	{
		if (itr->obj->model_ || itr->tag == "player" )
		{
			itr->obj->Draw();
		}
	}

}
void MCB::LevelLoader::LevelData::AnimationDraw()
{
	for (auto& itr : objects)
	{
		if (itr->obj->animationModel_) itr->obj->AnimationDraw();
	}
}

void MCB::LevelLoader::LevelData::DebugTextDraw(DebugText* debugText)
{
	for (auto& itr : objects)
	{
		itr->obj->DebugTextDraw(debugText);
	}
}

MCB::LevelLoader::LevelData::~LevelData()
{
	std::unordered_map<string, bool> deleteModel;
	for (auto& object : objects)
	{
		if (deleteModel.find(object->fileName) != deleteModel.end())
		{
			continue;
		}

		deleteModel[object->fileName] = true;
		if(object->obj->model_)object->obj->model_->isDelete_ = true;
		if (object->obj->animationModel_)object->obj->animationModel_->isDelete_ = true;
	}

	ModelManager::GetInstance()->erase();
}

std::unique_ptr<LevelLoader::LevelData> MCB::LevelLoader::LevelData::ReLoad()
{
	return Load(levelFileName,camera);
}

MCB::LevelLoader::LevelData::ObjectData::ObjectData()
{
}
