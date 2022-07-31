#pragma once
#include "IScene.h"
#define _USE_MATH_DEFINES
#include <cmath>


#pragma region �W��.h include

#include <d3dcompiler.h>
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <string>
#include <DirectXMath.h>
#include <wrl.h>
#include <cassert>
#include <memory>
#include <DirectXTex.h>
#include <array>

#pragma endregion �W��.h include

#pragma region ����.h include

#include "Input.h"
#include "DxWindow.h"
#include "Dx12.h"
#include "View.h"
#include "Projection.h"
#include "WorldMatrix.h"
#include "Depth.h"
#include "Object3d.h"
#include "ObjectMaterial.h"
#include "RootParameter.h"
#include "Vertex.h"
#include "MCBMatrix.h"
#include "Util.h"
#include "Particle.h"
#include "Quaternion.h"
#include "Texture.h"
#include "PIpelineRootSignature.h"
#include "Draw.h"
#include "Sprite.h"
#include "DebugText.h"
#include "Sound.h"
#include "Collider.h"
#include "Light.h"

#include "Human.h"
#pragma endregion ����.h include

#pragma region �Q�[���n.h include

#include "RayObject.h"
#include "SphereObj.h"

#pragma endregion �Q�[���n.h include

#pragma region pragma comment

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#pragma endregion pragma comment

namespace MCB
{
	class Scene :public IScene
	{
	private:
		#pragma region DirectX��b�@�\�Q
		DxWindow* dxWindow = DxWindow::GetInstance();
		//DirectX�N���X����
		Dx12* dx = Dx12::GetInstance();
		//input�N���X����
		Input* input = Input::GetInstance();

		ShaderResource* descriptor = ShaderResource::GetInstance();

		Depth* depth;
		//���[�g�p�����[�^
		RootParameter* rootparamsPtr;

		PipelineRootSignature* obj3dPipelinePtr;

		PipelineRootSignature* spritePipelinePtr;

		Light* light = nullptr;

		//�`��O�����ƕ`��㏈��
		MCB::Draw draw;

		//�T�E���h�}�l�[�W���[
		SoundManager soundManager;
		//�N���A�J���[
		float clearColor[4] = { 0.0f,0.25f, 0.5f,0.0f }; // ���ۂ��F
#pragma endregion DirectX��b�@�\�Q

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
#pragma endregion 3D���f��

		//�e�N�X�`��
		#pragma region �e�N�X�`��
		Texture testTex;
		Texture debugTextTexture;
		Texture zoomTex;
		Texture scopeTex;
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
		Human human;
		Object3d ground;
		Object3d Skydorm;
		Object3d box;
		RayObject ray;
		SphereObj sphere;

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
		Scene(RootParameter* root,Depth* depth, PipelineRootSignature* pipeline, PipelineRootSignature* pipeline1);
		~Scene();
		void Initialize() override;

		//�e�������n�֐��Q--------------------
		void LoadModel()  override;
		void LoadTexture()  override;
		void LoadSound()  override;
		void Object3DInit()  override;
		void SpriteInit()  override;

		//---------------
		void MatrixUpdate();
		void Update() override;
		void Draw() override;
	};

}

