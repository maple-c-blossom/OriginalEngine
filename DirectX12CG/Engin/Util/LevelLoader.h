#pragma once
#include <string>
#include <vector>
#include <DirectXMath.h>
#include <json.hpp>
#include "Object3d.h"
#include "PipelineManager.h"
namespace MCB
{
	class LevelLoader
	{
		public:
		// ���x���f�[�^
		struct LevelData {

			struct ObjectData {
				// �t�@�C����
				std::string fileName;
				std::string tag;
				std::string modelType;
				std::unique_ptr<Object3d> obj;
			};
			std::string levelFileName;
			ICamera* camera;
			// �I�u�W�F�N�g�z��
			std::vector<std::unique_ptr<ObjectData>> objects;
			Object3d* GetObjectPtr(std::string name);
			void Update(bool start = true);
			void UpdateMatrix();
			void Draw(PipeLineManager* pipeline,bool wireFrame = false);
			void AnimationDraw();
			void DebugTextDraw(DebugText* debugText);
			~LevelData();
			std::unique_ptr<LevelData> ReLoad();
		};
		//�萔
		// �f�t�H���g�̓ǂݍ��݃f�B���N�g��
		static const std::string sBASE_FILE_DIR_;
		// �t�@�C���g���q
		static const std::string sEXTEND_;

		public:// �����o�֐�

		static void RecursiveAnalysis(LevelData* levelData, nlohmann::json objJson,ICamera* camera);

		/// <summary>
		/// ���x���f�[�^�t�@�C���̓ǂݍ���
		/// </summary>
		/// <param name="fileName">�t�@�C����</param>
		static std::unique_ptr<LevelData> Load(const std::string& fileName, ICamera* camera);
		 
	};

}


