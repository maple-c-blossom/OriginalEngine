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
		DebugCamera debugCamera;
		
#pragma endregion 変換行列

#pragma region 各種リソース
		//3Dモデル
#pragma region 3Dモデル
		std::unique_ptr<Model> SpherModel = nullptr;
		std::unique_ptr<Model> SpherModel2 = nullptr;
		std::unique_ptr<Model> groundModel = nullptr;
		std::unique_ptr<Model> skydomeModel = nullptr;

		//AssimpLoader testModel;
#pragma endregion 3Dモデル

		//テクスチャ
#pragma region テクスチャ
		TextureCell* debugTextTexture = nullptr;
		TextureCell* zoomTex = nullptr;

#pragma endregion テクスチャ

		//サウンド
#pragma region サウンド
		int testSound = -1;
		int test2Sound = -1;

		int volume = 255;
#pragma endregion サウンド

#pragma endregion 各種リソース

#pragma region 3Dオブジェクト
		SimpleFigure triangle;
		
		Object3d ground = {};
		Object3d Skydorm = {};
		Object3d testsphere = {};
#pragma endregion 3Dオブジェクト

#pragma region スプライト
		Sprite sprite = {};

		Sprite zoomSprite = {};

		Sprite scopeSprite = {};

		DebugText debugText = {};

#pragma endregion スプライト
#pragma region パーティクル

#pragma endregion
#pragma region 通常変数
		bool loopFlag = true;
		bool startPositionReset = true;
		bool ybill = false;
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
		std::shared_ptr<IScene> GetNextScene() override;
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

