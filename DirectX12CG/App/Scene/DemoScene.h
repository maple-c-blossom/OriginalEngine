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
#pragma region �ϊ��s��
		//�ϊ��s��
		DebugCamera camera_;
#pragma endregion �ϊ��s��

#pragma region �e�탊�\�[�X
		//3D���f��
#pragma region 3D���f��
		std::unique_ptr<Model> groundModel_;
		std::unique_ptr<Model> skydomeModel_;
		std::unique_ptr<Model> sphereModel_;
		std::unique_ptr<Model> boxModel_;

		std::unique_ptr<AnimationModel> animModel_;
		std::unique_ptr<AnimationModel> anim2Model_;
#pragma endregion 3D���f��

		//�e�N�X�`��
#pragma region �e�N�X�`��
		TextureCell* debugTextTexture_;
		TextureCell* titleTex_;

#pragma endregion �e�N�X�`��
		bool debugView_;
		std::array<bool,4> isIk_;
		std::array<bool,4> noMove;
		//�T�E���h
#pragma region �T�E���h
		size_t selectSound_;
		size_t test2Sound_;
		
		int32_t volume_ = 255;
#pragma endregion �T�E���h

#pragma endregion �e�탊�\�[�X

#pragma region 3D�I�u�W�F�N�g
		Object3d ground_;
		Object3d Skydorm_;
		std::array<Object3d,4> effectorObjects_;
		Object3d test2Animation_;
		Player play_;
		std::array<DirectX::XMFLOAT3,4> poleVec_;
		std::array<bool, 4> PoleVecMove_ = { false,false,false,false };
		bool objChenge_;
		std::array<std::string, 4> ikBoneName_ = {"Wrist.L","Wrist.R","Foot.L","Foot.R"};
#pragma endregion 3D�I�u�W�F�N�g

#pragma region �X�v���C�g
		Sprite sprite_;

		Sprite titleSprite_;

		Sprite scopeSprite_;

		DebugText debugText_;

#pragma endregion �X�v���C�g

#pragma region �ʏ�ϐ�
		bool loopFlag_ = true;
		bool startPositionReset_ = true;
		bool debugStop = false;
#pragma endregion �ʏ�ϐ�
	public:

		DemoScene(RootParameter* root, Depth* depth,PipeLineManager* pipeline);
		~DemoScene();
		void Initialize() override;

		//�e�������n�֐��Q--------------------
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

