#pragma once
#include "Dx12.h"
#include <string>
#include <memory>
#include "ObjectMaterial.h"
#include "Texture.h"
namespace MCB
{
    //���_�f�[�^�\����-------------------------------------
    typedef struct FbxVertex
    {
        Float3 pos;//xyz���W
        Float3 normal;//�@���x�N�g��
        Float2 uv;//uv���W
    };
    //--------------------------------------
    typedef struct Mesh
    {
        std::vector<FbxVertex> vertices;
        std::vector<uint32_t> indices;
        std::wstring DiffuseMap;
    };


    typedef struct ImportSetting
    {
        const std::string fileName;//�t�@�C���p�X
        std::vector<Mesh>& meshies; //�o�͐惁�b�V���z��
        bool inversU = false; //U���W���]�t���O
        bool inversV = false; //V���W���]�t���O
    };

    struct aiMesh;
    struct aiMaterial;

    class AssimpLoader
    {
    private:
        AssimpLoader() {};
        AssimpLoader(const AssimpLoader& dx12) {};
        AssimpLoader& operator=(const AssimpLoader& dx12) {};
        ~AssimpLoader() {  };
        using string = std::string;
        ID3D12Device* device = nullptr;
        void LoadMesh(Mesh& dst,const aiMesh* src,bool inversU, bool inverV);

    public:
        static const string baseDirectory;
        static void DeleteInstance();
        static AssimpLoader* GetInstance();
        static AssimpLoader* GetInitInstance();
        void Initialize();
        void Finalize();

        bool LoadFile(ImportSetting setting);
    };
}
