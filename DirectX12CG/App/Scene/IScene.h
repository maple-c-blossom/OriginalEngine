#pragma once

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
#include "LightGroup.h"
#include "FBXLoader.h"
#include "PipeLineManager.h"
#include "ImguiManager.h"
#include "Particle.h"//��XParticleManager�ɕύX�BParticle�P�̂Ńe�X�g����p
#include "ICamera.h"
#include "TextureManager.h"
#include <MyImgui/imgui.h>
#pragma endregion ����.h include
#pragma region pragma comment

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#pragma endregion pragma comment

namespace MCB
{
	class IScene
	{

	protected:
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

		PipeLineManager* pipeline;

		//PipelineRootSignature* obj3dPipelinePtr;

		//PipelineRootSignature* spritePipelinePtr;

		//PipelineRootSignature* particlePipelinePtr;

		LightGroup* lights = LightGroup::GetInstance();

		//�`��O�����ƕ`��㏈��
		MCB::Draw* draw = Draw::GetInstance();

		//FBXLoader* fbxLoader = FBXLoader::GetInstance();
	
		//�T�E���h�}�l�[�W���[
		SoundManager soundManager;
		//�N���A�J���[
#pragma endregion DirectX��b�@�\�Q
		bool sceneEnd = false;

		IScene* nextScene = nullptr;

		ICamera* viewCamera;

		TextureManager* loader = TextureManager::GetInstance();
	public:
		ImguiManager imgui;
		float clearColor[4] = { 0.0f,0.25f, 0.5f,0.0f }; // ���ۂ��F
		virtual ~IScene() {};
		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void Draw() = 0;
		virtual void SpriteDraw() = 0;
		virtual void ParticleDraw() = 0;
		virtual void LoadModel() = 0;
		virtual void LoadTexture() = 0;
		virtual void LoadSound() = 0;
		virtual void Object3DInit() = 0;
		virtual void SpriteInit() = 0;
		virtual void ParticleInit() = 0;
		virtual void MatrixUpdate() = 0;
		void ImGuiDraw() { imgui.Draw(); }
		virtual void ImGuiUpdate() = 0;
		virtual IScene* GetNextScene() = 0;
		virtual void CheckAllColision() = 0;
		bool GetIsSceneEnd() { return sceneEnd; }

		Depth* GetDepth() { return depth; }
		//���[�g�p�����[�^
		RootParameter* GetRootparamsPtr() { return rootparamsPtr; }
		
		PipeLineManager* Getpipeline() { return pipeline; }
	};

}

