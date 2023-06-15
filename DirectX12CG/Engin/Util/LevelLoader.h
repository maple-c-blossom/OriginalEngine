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
		// レベルデータ
		struct LevelData {

			struct ObjectData {
				// ファイル名
				std::string fileName;
				Object3d obj;
			};

			// オブジェクト配列
			std::vector<ObjectData> objects;
		};

		public:// 定数
			// デフォルトの読み込みディレクトリ
			static const std::string sBASE_FILE_DIR_;
			// ファイル拡張子
			static const std::string sEXTEND_;

		public:// メンバ関数

			static void RecursiveAnalysis(LevelData* levelData, nlohmann::json objJson);

			/// <summary>
			/// レベルデータファイルの読み込み
			/// </summary>
			/// <param name="fileName">ファイル名</param>
			static std::unique_ptr<LevelData>  Load(const std::string& fileName);
	};

}


