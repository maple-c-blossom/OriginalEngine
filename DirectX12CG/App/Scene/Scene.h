#pragma once
#include "IScene.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include "DebugCamera.h"
#include "Player.h"
#include "Goal.h"
#include "CheckPoint.h"
#include "Camera.h"
#include "StartCamera.h"
#include <memory>

#pragma region ゲーム系.h include


#pragma endregion ゲーム系.h include


namespace MCB
{

	class Scene :public IScene
	{
	private:
		int64_t pad;
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
		Camera maincamera_;
		StartCamera startcamera;
		std::string stage;
#pragma endregion 変換行列

#pragma region 各種リソース
		//3Dモデル
#pragma region 3Dモデル
		Model* SpherModel_ = nullptr;
		AnimationModel* playerModel_ = nullptr;
		Model* groundModel_ = nullptr;
		Model* skydomeModel_ = nullptr;
		Model* goalModel_ = nullptr;
		//AssimpLoader testModel;
#pragma endregion 3Dモデル

		//テクスチャ
#pragma region テクスチャ
		TextureCell* debugTextTexture_ = nullptr;
		TextureCell* goText = nullptr;
		TextureCell* moveText = nullptr;
		TextureCell* lStickTex = nullptr;
		TextureCell* jumpTex = nullptr;
		TextureCell* numTex = nullptr;
		TextureCell* startTex = nullptr;

#pragma endregion テクスチャ

		//サウンド
#pragma region サウンド
		size_t runNormal;
		size_t runFast;

		int32_t volume_ = 255;
		Byte4 pad2;
#pragma endregion サウンド

#pragma endregion 各種リソース

#pragma region 3Dオブジェクト

		Goal* goal_;
		Player* player_;
		Object3d* skydome;
		int64_t pad3;
#pragma endregion 3Dオブジェクト

#pragma region スプライト
		Sprite sprite_ = {};
		Sprite countSprite_ = {};

		Sprite moveTextSprite_ = {};

		Sprite lStickTexSprite_ = {};
		Sprite jumpTexSprite_ = {};

		Sprite startTexSprite_ = {};

		DebugText debugText_ = {};
		std::array<Float2,4> stickMove;
		Float2 centerPos = { 200,dxWindow_->sWINDOW_HEIGHT_ - 100.f };
		Float2 stickPos = { 200,dxWindow_->sWINDOW_HEIGHT_ - 100.f };
		Float2 nextStickPos = { 200,dxWindow_->sWINDOW_HEIGHT_ - 68.f };
		Float2 prevStickPos = { 200,dxWindow_->sWINDOW_HEIGHT_ - 68.f };

		int movenum = 0;
		Timer stickMoveTime;
#pragma endregion スプライト
#pragma region パーティクル

#pragma endregion
#pragma region 通常変数
		bool loopFlag_ = true;
		bool startPositionReset_ = true;
		bool ybill_ = false;
		bool debugWindow_ = false;
		Byte4 pad4;
		std::unique_ptr<LevelLoader::LevelData> level_;
		Timer startTimer;
		bool startDrawed;
		Byte7 pad5;
#pragma endregion 通常変数

	public:
		Scene(RootParameter* root,Depth* depth,PipeLineManager* pipeline);
		Scene(const Scene&) =delete;
		Scene& operator=(const Scene&) = delete;
		~Scene();
		void Initialize() override;
		void SetStage(std::string stageName);
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

