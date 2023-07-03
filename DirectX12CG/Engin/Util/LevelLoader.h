#pragma once
#include <string>
#include <vector>
#include <DirectXMath.h>
#include <json.hpp>
#include "Object3d.h"

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
				Object3d obj;
			};
			std::string levelFileName;
			ICamera* camera;
			// �I�u�W�F�N�g�z��
			std::vector<std::unique_ptr<ObjectData>> objects;
			void Update();
			void UpdateMatrix();
			void Draw();
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
		static std::unique_ptr<LevelData>  Load(const std::string& fileName, ICamera* camera);
		 
	};

}


