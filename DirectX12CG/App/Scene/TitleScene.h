#pragma once
#include "IScene.h"
#define _USE_MATH_DEFINES
#include "IgnoreWarning.h"
WarningIgnoreBegin
#include <cmath>
WarningIgnoreEnd
#include "DebugCamera.h"
#include "Player.h"
namespace MCB
{
	class TitleScene :public IScene
	{
	private:
		int64_t pad;
#pragma region 変換行列
		//変換行列
		DebugCamera camera_;
#pragma endregion 変換行列

#pragma region 各種リソース
		//3Dモデル
#pragma region 3Dモデル
		std::unique_ptr<Model> groundModel_;
		std::unique_ptr<Model> skydomeModel_;
#pragma endregion 3Dモデル

		//テクスチャ
#pragma region テクスチャ
		TextureCell* debugTextTexture_;
		TextureCell* titleTex_;
		TextureCell* backGroundTex_;
		TextureCell* abuttonTex_;

#pragma endregion テクスチャ
		bool debugView;
		bool isIk;
		Byte6 pad2;
		//サウンド
#pragma region サウンド
		size_t selectSound_;
		size_t test2Sound_;

		int32_t volume_ = 255;
		Byte6 pad3;
		Byte6 pad4;
#pragma endregion サウンド

#pragma endregion 各種リソース

#pragma region 3Dオブジェクト
		Object3d ground_;
		Object3d Skydorm_;

		bool PoleVecMove;
		Byte3 pad5;
#pragma endregion 3Dオブジェクト

#pragma region スプライト
		Sprite sprite_;

		Sprite titleSprite_;

		Sprite aButtonSprite_;

		DebugText debugText_;

#pragma endregion スプライト

#pragma region 通常変数
		bool loopFlag_ = true;
		bool startPositionReset_ = true;
		bool debugStop = false;
		Byte1 pad6;
		Timer titleMoveTime_;
		Byte4 pad7;
#pragma endregion 通常変数
	public:

		TitleScene(RootParameter* root, Depth* depth,PipeLineManager* pipeline);
		TitleScene(const TitleScene&)=delete;
		TitleScene& operator=(const TitleScene&) = delete;
		~TitleScene();
		void Initialize() override;

		//各初期化系関数群--------------------
		void LoadModel()  override;
		void LoadTexture()  override;
		void LoadSound()  override;
		void Object3DInit()  override;
		void SpriteInit()  override;
		void ParticleInit()  override;
		std::unique_ptr<IScene> GetNextScene() override;
		//---------------
		void MatrixUpdate() override;
		void Update() override;
		void PostEffectDraw() override;
		void Draw() override;
		void SpriteDraw() override;
		void ParticleDraw() override;
		void CheckAllColision()override;
		void ImGuiUpdate()override;
	};
}

