#pragma once
#include "Model.h"
#include "AnimationModel.h"
#include "IgnoreWarning.h"
WarningIgnoreBegin
#include <unordered_map>
WarningIgnoreEnd
namespace MCB
{
    class ModelManager
    {
    private:

        ModelManager();
        ModelManager(const ModelManager& modelManager) = delete;
        ModelManager& operator=(const ModelManager& modelManager) = delete;
		~ModelManager();
        static std::unordered_map<std::string, std::unique_ptr<Model>> modelMap_;
        static std::unordered_map<std::string, std::unique_ptr<AnimationModel>> animModelMap_;
    public:
        static ModelManager* GetInstance();
        static ModelManager* GetInitInstance();
        static void Init();
        void erase(const std::string& fileName, const std::string& extend = "obj");
        void erase();//isDelete_がtrueのModelをすべて削除
        static void Load(const std::string& fileName,bool smooth = false, const std::string& extend = "obj");
        static Model* GetModel(const std::string& fileName, bool smooth = false);
        static AnimationModel* GetModel(const std::string& fileName, AnimationModel* ptr);
    };
}