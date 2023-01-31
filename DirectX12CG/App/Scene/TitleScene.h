#pragma once
#include "IScene.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include "Camera.h"
namespace MCB
{
	class TitleScene :public IScene
	{
	private:
#pragma region 変換行列
		//変換行列
		Camera camera;
#pragma endregion 変換行列

#pragma region 各種リソース
		//3Dモデル
#pragma region 3Dモデル
		std::unique_ptr<Model> groundModel;
		std::unique_ptr<Model> skydomeModel;

		std::unique_ptr<AnimationModel> animModel;
		std::unique_ptr<AnimationModel> anim2Model;
#pragma endregion 3Dモデル

		//テクスチャ
#pragma region テクスチャ
		TextureCell* debugTextTexture;

#pragma endregion テクスチャ

		//サウンド
#pragma region サウンド
		int testSound;
		int test2Sound;

		int volume = 255;
#pragma endregion サウンド

#pragma endregion 各種リソース

#pragma region 3Dオブジェクト
		SimpleFigure triangle;
		Object3d ground;
		Object3d Skydorm;
		Object3d testAnimation;
		Object3d test2Animation;


#pragma endregion 3Dオブジェクト

#pragma region スプライト
		Sprite sprite;

		Sprite zoomSprite;

		Sprite scopeSprite;

		DebugText debugText;

#pragma endregion スプライト

#pragma region 通常変数
		bool loopFlag = true;
		bool startPositionReset = true;
#pragma endregion 通常変数
	public:

		TitleScene(RootParameter* root, Depth* depth,PipeLineManager* pipeline);
		~TitleScene();
		void Initialize() override;

		//各初期化系関数群--------------------
		void LoadModel()  override;
		void LoadTexture()  override;
		void LoadSound()  override;
		void Object3DInit()  override;
		void SpriteInit()  override;
		void ParticleInit()  override;
		IScene* GetNextScene() override;
		//---------------
		void MatrixUpdate() override;
		void Update() override;
		void Draw() override;
		void SpriteDraw() override;
		void ParticleDraw() override;
		void CheckAllColision()override;
		void ImGuiUpdate()override;
	};
}

