#pragma once
#include "IScene.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include "DebugCamera.h"
#include "Player.h"
#include <memory>
#pragma region �Q�[���n.h include


#pragma endregion �Q�[���n.h include


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

		//�ϊ��s��
		DebugCamera debugCamera;
		
#pragma endregion �ϊ��s��

#pragma region �e�탊�\�[�X
		//3D���f��
#pragma region 3D���f��
		std::unique_ptr<Model> SpherModel = nullptr;
		std::unique_ptr<Model> SpherModel2 = nullptr;
		std::unique_ptr<Model> groundModel = nullptr;
		std::unique_ptr<Model> skydomeModel = nullptr;

		//AssimpLoader testModel;
#pragma endregion 3D���f��

		//�e�N�X�`��
#pragma region �e�N�X�`��
		TextureCell* debugTextTexture = nullptr;
		TextureCell* zoomTex = nullptr;

#pragma endregion �e�N�X�`��

		//�T�E���h
#pragma region �T�E���h
		int testSound = -1;
		int test2Sound = -1;

		int volume = 255;
#pragma endregion �T�E���h

#pragma endregion �e�탊�\�[�X

#pragma region 3D�I�u�W�F�N�g
		SimpleFigure triangle;
		
		Object3d ground = {};
		Object3d Skydorm = {};
		Object3d testsphere = {};
#pragma endregion 3D�I�u�W�F�N�g

#pragma region �X�v���C�g
		Sprite sprite = {};

		Sprite zoomSprite = {};

		Sprite scopeSprite = {};

		DebugText debugText = {};

#pragma endregion �X�v���C�g
#pragma region �p�[�e�B�N��

#pragma endregion
#pragma region �ʏ�ϐ�
		bool loopFlag = true;
		bool startPositionReset = true;
		bool ybill = false;
#pragma endregion �ʏ�ϐ�

	public:
		Scene(RootParameter* root,Depth* depth,PipeLineManager* pipeline);
		~Scene();
		void Initialize() override;

		//�e�������n�֐��Q--------------------
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

