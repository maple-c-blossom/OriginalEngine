#include "ModelManager.h"
using namespace MCB;
using namespace std;

std::unordered_map<std::string, std::unique_ptr<Model>> ModelManager::modelMap_ = {};
std::unordered_map<std::string, std::unique_ptr<AnimationModel>> ModelManager::animModelMap_ = {};
ModelManager* ModelManager::GetInstance()
{
    static ModelManager inst;
    return &inst;
}

ModelManager* MCB::ModelManager::GetInitInstance()
{
    static ModelManager inst;
    inst.Init();
    return &inst;
}

void MCB::ModelManager::Init()
{
    modelMap_.clear();
    animModelMap_.clear();
}

void MCB::ModelManager::erase(const string& fileName, const string& extend)
{
    if (extend == "gltf")
    {
        animModelMap_.erase(fileName);
    }
    else
    {
        modelMap_.erase(fileName);
    }
}

void MCB::ModelManager::erase()
{
    for (auto& itr : modelMap_)
    {
        if (itr.second->isDelete_)
        {
            modelMap_.erase(itr.second->fileName_);
        }
    }

    for (auto& itr : animModelMap_)
    {
        if (itr.second->isDelete_)
        {
            animModelMap_.erase(itr.second->fileName_);
        }
    }
}

void MCB::ModelManager::Load(const string& fileName, bool smooth,const string& extend)
{
    if (extend == "gltf")
    {
        if (animModelMap_.find(fileName) == animModelMap_.end())
        {
            unique_ptr<AnimationModel> temp = make_unique<AnimationModel>();
            temp->Load(fileName);
            animModelMap_[fileName] = move(temp);
        }
    }
    else if (extend == "obj")
    {
        if (modelMap_.find(fileName) == modelMap_.end())
        {
            unique_ptr<Model> temp = make_unique<Model>(fileName,smooth);
            modelMap_[fileName] = move(temp);
        }
    }
}

Model* MCB::ModelManager::GetModel(const string& fileName, bool smooth)
{
    auto itr = modelMap_.find(fileName);
    if (itr == modelMap_.end())
    {
        Load(fileName,smooth);
    }
    return modelMap_[fileName].get();
}

AnimationModel* MCB::ModelManager::GetModel(const string& fileName, AnimationModel* ptr)
{
    auto itr = animModelMap_.find(fileName);
    if (itr == animModelMap_.end())
    {
        Load(fileName, false,"gltf");
    }
    return animModelMap_[fileName].get();
}
