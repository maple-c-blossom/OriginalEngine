#pragma once
#include "IgnoreWarning.h"
WarningIgnoreBegin
#include <string>
#include <vector>
#include <DirectXMath.h>
#include <json.hpp>
WarningIgnoreEnd
#include "Object3d.h"
#include "PipelineManager.h"
namespace MCB
{
	class LevelLoader
	{
	public:
	// レベルデータ
		struct LevelData
		{


			struct ObjectData
			{
// ファイル名
				std::string fileName;
				std::string tag;
				std::string modelType;
				std::unique_ptr<Object3d> obj;
				ObjectData();
				ObjectData(const ObjectData&) = delete;
				ObjectData& operator=(const ObjectData&) = delete;

			};
			std::string levelFileName;
			ICamera* camera;
			// オブジェクト配列
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
		//定数
		// デフォルトの読み込みディレクトリ
		static const std::string sBASE_FILE_DIR_;
		// ファイル拡張子
		static const std::string sEXTEND_;

		static const std::array<Float3,3> colors;
		public:// メンバ関数

		static void RecursiveAnalysis(LevelData* levelData, nlohmann::json objJson,ICamera* camera);

		/// <summary>
		/// レベルデータファイルの読み込み
		/// </summary>
		/// <param name="fileName">ファイル名</param>
		static std::unique_ptr<LevelData> Load(const std::string& fileName, ICamera* camera);
		 
	};

}


