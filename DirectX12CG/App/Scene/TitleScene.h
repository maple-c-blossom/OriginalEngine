#pragma once
#include "IScene.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include "DebugCamera.h"
#include "Player.h"
namespace MCB
{
	class TitleScene :public IScene
	{
	private:
#pragma region �ϊ��s��
		//�ϊ��s��
		DebugCamera camera;
#pragma endregion �ϊ��s��

#pragma region �e�탊�\�[�X
		//3D���f��
#pragma region 3D���f��
		std::unique_ptr<Model> groundModel;
		std::unique_ptr<Model> skydomeModel;
		std::unique_ptr<Model> sphereModel;

		std::unique_ptr<AnimationModel> animModel;
		std::unique_ptr<AnimationModel> anim2Model;
#pragma endregion 3D���f��

		//�e�N�X�`��
#pragma region �e�N�X�`��
		TextureCell* debugTextTexture;

#pragma endregion �e�N�X�`��

		//�T�E���h
#pragma region �T�E���h
		int testSound;
		int test2Sound;

		int volume = 255;
#pragma endregion �T�E���h

#pragma endregion �e�탊�\�[�X

#pragma region 3D�I�u�W�F�N�g
		SimpleFigure triangle;
		Object3d ground;
		Object3d Skydorm;
		Object3d testsphere;
		Object3d test2Animation;
		Player play;

#pragma endregion 3D�I�u�W�F�N�g

#pragma region �X�v���C�g
		Sprite sprite;

		Sprite zoomSprite;

		Sprite scopeSprite;

		DebugText debugText;

#pragma endregion �X�v���C�g

#pragma region �ʏ�ϐ�
		bool loopFlag = true;
		bool startPositionReset = true;
#pragma endregion �ʏ�ϐ�
	public:

		TitleScene(RootParameter* root, Depth* depth,PipeLineManager* pipeline);
		~TitleScene();
		void Initialize() override;

		//�e�������n�֐��Q--------------------
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
		void PostEffectDraw() override;
		void Draw() override;
		void SpriteDraw() override;
		void ParticleDraw() override;
		void CheckAllColision()override;
		void ImGuiUpdate()override;
	};
}

