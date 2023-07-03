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
#include "CollisionManager.h"
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
#include "Particle.h"
#include "ICamera.h"
#include "TextureManager.h"
#include <MyImgui/imgui.h>
#include "SphereCollider.h"
#include "PlaneCollider.h"
#include "RayCollider.h"
#include "TriangleCollider.h"
#include "BoxCollider.h"
#include "MeshCollider.h"
#include "ModelManager.h"

#include "LevelLoader.h"
#include "PostEffect.h"
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
		DxWindow* dxWindow_ = DxWindow::GetInstance();
		//DirectX�N���X����
		Dx12* dx_ = Dx12::GetInstance();
		//input�N���X����
		Input* input_ = Input::GetInstance();

		ShaderResource* descriptor_ = ShaderResource::GetInstance();

		Depth* depth_;
		//���[�g�p�����[�^
		RootParameter* rootparamsPtr_;

		PipeLineManager* pipeline_;

		//PipelineRootSignature* obj3dPipelinePtr;

		//PipelineRootSignature* spritePipelinePtr;

		//PipelineRootSignature* particlePipelinePtr;

		LightGroup* lights_ = LightGroup::GetInstance();

		//�`��O�����ƕ`��㏈��
		MCB::Draw* draw_ = Draw::GetInstance();

		//FBXLoader* fbxLoader = FBXLoader::GetInstance();
	
		//�T�E���h�}�l�[�W���[
		SoundManager* soundManager_ = SoundManager::GetInstance();
		ModelManager* modelManager_ = ModelManager::GetInstance();
		//�N���A�J���[
#pragma endregion DirectX��b�@�\�Q
		bool sceneEnd_ = false;

		std::shared_ptr<IScene> nextScene_ = nullptr;

		ICamera* viewCamera_;

		TextureManager* loader_ = TextureManager::GetInstance();

		std::unique_ptr<PostEffect> postEffect_ = std::make_unique<PostEffect>();
	public:
		ImguiManager imgui_;
		float clearColor_[4] = { 0.0f,0.25f, 0.5f,0.0f }; // ���ۂ��F
		virtual ~IScene() {};
		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void Draw() = 0;
		virtual void PostEffectDraw() = 0;
		virtual void SpriteDraw() = 0;
		virtual void ParticleDraw() = 0;
		virtual void LoadModel() = 0;
		virtual void LoadTexture() = 0;
		virtual void LoadSound() = 0;
		virtual void Object3DInit() = 0;
		virtual void SpriteInit() = 0;
		virtual void ParticleInit() = 0;
		virtual void MatrixUpdate() = 0;
		void ImGuiDraw() { imgui_.Draw(); }
		virtual void ImGuiUpdate() = 0;
		virtual std::unique_ptr<IScene> GetNextScene() = 0;
		virtual void CheckAllColision() = 0;
		bool GetIsSceneEnd() { return sceneEnd_; }

		Depth* GetDepthPtr() { return depth_; }
		Depth& GetDepth() { return *depth_; }
		//���[�g�p�����[�^
		RootParameter* GetRootparamsPtr() { return rootparamsPtr_; }
		
		PipeLineManager* GetpipelinePtr() { return pipeline_; }
		PipeLineManager& Getpipeline() { return *pipeline_; }
	};

}

