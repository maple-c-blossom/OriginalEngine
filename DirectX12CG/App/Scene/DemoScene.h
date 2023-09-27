#pragma once
#include "IScene.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include "DebugCamera.h"
#include "Player.h"
namespace MCB
{
	class DemoScene :public IScene
	{
	private:
#pragma region 変換行列
		//変換行列
		DebugCamera camera_;
#pragma endregion 変換行列

#pragma region 各種リソース
		//3Dモデル
#pragma region 3Dモデル
		std::unique_ptr<Model> groundModel_;
		std::unique_ptr<Model> skydomeModel_;
		std::unique_ptr<Model> sphereModel_;
		std::unique_ptr<Model> boxModel_;

		std::unique_ptr<AnimationModel> animModel_;
		std::unique_ptr<AnimationModel> anim2Model_;
#pragma endregion 3Dモデル

		//テクスチャ
#pragma region テクスチャ
		TextureCell* debugTextTexture_;
		TextureCell* titleTex_;

#pragma endregion テクスチャ
		bool debugView_;
		std::array<bool,4> isIk_;
		std::array<bool,4> noMove;
		//サウンド
#pragma region サウンド
		size_t selectSound_;
		size_t test2Sound_;
		
		int32_t volume_ = 255;
#pragma endregion サウンド

#pragma endregion 各種リソース

#pragma region 3Dオブジェクト
		Object3d ground_;
		Object3d Skydorm_;
		std::array<Object3d,4> effectorObjects_;
		Object3d test2Animation_;
		Player play_;
		std::array<DirectX::XMFLOAT3,4> poleVec_;
		std::array<bool, 4> PoleVecMove_ = { false,false,false,false };
		bool objChenge_;
		std::array<std::string, 4> ikBoneName_ = {"Wrist.L","Wrist.R","Foot.L","Foot.R"};
#pragma endregion 3Dオブジェクト

#pragma region スプライト
		Sprite sprite_;

		Sprite titleSprite_;

		Sprite scopeSprite_;

		DebugText debugText_;

#pragma endregion スプライト

#pragma region 通常変数
		bool loopFlag_ = true;
		bool startPositionReset_ = true;
		bool debugStop = false;
#pragma endregion 通常変数
	public:

		DemoScene(RootParameter* root, Depth* depth,PipeLineManager* pipeline);
		~DemoScene();
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

