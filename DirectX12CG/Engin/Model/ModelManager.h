#pragma once
#include "Model.h"
#include "FbxLoader.h"
#include <unordered_map>
namespace MCB
{
    class ModelManager
    {
    private:
        ModelManager() {};
        ModelManager(const ModelManager& dx12) {};
        ModelManager& operator=(const ModelManager& dx12) {};
        ~ModelManager() {};
        static std::unordered_map<std::string, std::unique_ptr<Model>> modelMap_;
        static std::unordered_map<std::string, std::unique_ptr<AnimationModel>> animModelMap_;
    public:
        static ModelManager* GetInstance();
        static ModelManager* GetInitInstance();
        static void Init();
        void erase(const std::string& fileName, const std::string& extend = "obj");
        void erase();//isDelete_‚ªtrue‚ÌModel‚ğ‚·‚×‚Äíœ
        static void Load(const std::string& fileName,bool smooth = false, const std::string& extend = "obj");
        static Model* GetModel(const std::string& fileName, bool smooth = false);
        static AnimationModel* GetModel(const std::string& fileName, AnimationModel* ptr);
    };
}