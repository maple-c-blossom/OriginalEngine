#pragma once
#include "IScene.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include "DebugCamera.h"
#include "Player.h"
#include <memory>
#pragma region ゲーム系.h include


#pragma endregion ゲーム系.h include


namespace MCB
{
	class Scene :public IScene
	{
	private:

		enum PostEffectNum
		{
			STRIPE = 1,
			AVARAGEBLER = 2,
			INV = 3,
			GAUSSIANBLUR = 4,
			GRAY = 5,
			SEPIA = 6,
			NONE = 7
		};

		//変換行列
		DebugCamera debugCamera_;
		
#pragma endregion 変換行列

#pragma region 各種リソース
		//3Dモデル
#pragma region 3Dモデル
		std::unique_ptr<Model> SpherModel_ = nullptr;
		std::unique_ptr<Model> SpherModel2_ = nullptr;
		std::unique_ptr<Model> groundModel_ = nullptr;
		std::unique_ptr<Model> skydomeModel_ = nullptr;

		//AssimpLoader testModel;
#pragma endregion 3Dモデル

		//テクスチャ
#pragma region テクスチャ
		TextureCell* debugTextTexture_ = nullptr;
		TextureCell* zoomTex_ = nullptr;

#pragma endregion テクスチャ

		//サウンド
#pragma region サウンド
		size_t testSound_ = -1;
		size_t test2Sound_ = -1;

		int32_t volume_ = 255;
#pragma endregion サウンド

#pragma endregion 各種リソース

#pragma region 3Dオブジェクト
		SimpleFigure triangle_;
		Player player_;

		Object3d ground_ = {};
		Object3d Skydorm_ = {};
		Object3d testsphere_ = {};
#pragma endregion 3Dオブジェクト

#pragma region スプライト
		Sprite sprite_ = {};

		Sprite zoomSprite_ = {};

		Sprite scopeSprite_ = {};

		DebugText debugText_ = {};

#pragma endregion スプライト
#pragma region パーティクル

#pragma endregion
#pragma region 通常変数
		bool loopFlag_ = true;
		bool startPositionReset_ = true;
		bool ybill_ = false;
#pragma endregion 通常変数

	public:
		Scene(RootParameter* root,Depth* depth,PipeLineManager* pipeline);
		~Scene();
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
		void MatrixUpdate()override;
		void Update() override;
		void Draw() override;
		void PostEffectDraw() override;
		void SpriteDraw() override;
		void ParticleDraw()override;
		void CheckAllColision()override;
		void ImGuiUpdate()override;
	};

}

