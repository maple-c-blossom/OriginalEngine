#pragma once
#include "Dx12.h"
#include "fbxsdk.h"
#include <string>
namespace MCB
{
    class FBXLoader
    {
    private:
        FBXLoader() {};
        FBXLoader(const FBXLoader& dx12) {};
        FBXLoader& operator=(const FBXLoader& dx12) {};
        ~FBXLoader() { Finalize(); };
        using string = std::string;
        ID3D12Device* device = nullptr;
        FbxManager* manager = nullptr;
        FbxImporter* fbxImporter = nullptr;
     

    public:
        static const string baseDirectory;
        static void DeleteInstance();
        static FBXLoader* GetInstance();
        static FBXLoader* GetInitInstance();
        void Initialize();
        void Finalize();

        void LoadModelFromFile(const string& modelName);
    };
}
