#pragma once
#include "IScene.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include "DebugCamera.h"

#pragma region �Q�[���n.h include


#pragma endregion �Q�[���n.h include


namespace MCB
{
	class Scene :public IScene
	{
	private:
		#pragma region �ϊ��s��
		//�ϊ��s��
		View matView;
		Projection matProjection;
#pragma endregion �ϊ��s��
		
		#pragma region �e�탊�\�[�X
		//3D���f��
		#pragma region 3D���f��
		Model* BoxModel;
		Model* groundModel;
		Model* skydomeModel;

		FBXModel* testModel;
#pragma endregion 3D���f��

		//�e�N�X�`��
		#pragma region �e�N�X�`��
		TextureCell* testTex;
		TextureCell* debugTextTexture;
		TextureCell* zoomTex;
		TextureCell* scopeTex;
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
		Object3d testSpher;

#pragma endregion 3D�I�u�W�F�N�g

		#pragma region �X�v���C�g
		Sprite sprite;

		Sprite zoomSprite;

		Sprite scopeSprite;

		DebugText debugText;

#pragma endregion �X�v���C�g
#pragma region �p�[�e�B�N��
		Particle testParticle;
		

#pragma endregion
		#pragma region �ʏ�ϐ�
		bool loopFlag = true;
		bool startPositionReset = true;
		bool ybill = false;
		DebugCamera debugCamera;
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
		IScene* GetNextScene() override;
		//---------------
		void MatrixUpdate()override;
		void Update() override;
		void Draw() override;
		void SpriteDraw() override;
		void ParticleDraw()override;
		void CheckAllColision()override;
		void ImGuiUpdate()override;
	};

}

