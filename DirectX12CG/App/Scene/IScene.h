#pragma once
#include"IgnoreWarning.h"
#pragma region 標準.h include
WarningIgnoreBegin
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
#include <MyImgui/imgui.h>
WarningIgnoreEnd
#pragma endregion 標準.h include

#pragma region 自作.h include
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
#include "AnimationModel.h"
#include "PipeLineManager.h"
#include "ImguiManager.h"
#include "Particle.h"
#include "ICamera.h"
#include "TextureManager.h"
#include "SphereCollider.h"
#include "PlaneCollider.h"
#include "RayCollider.h"
#include "TriangleCollider.h"
#include "BoxCollider.h"
#include "MeshCollider.h"
#include "ModelManager.h"
#include "Timer.h"
#include "LevelLoader.h"
#include "PostEffect.h"
#pragma endregion 自作.h include


namespace MCB
{

	class IScene
	{

	protected:
#pragma region DirectX基礎機能群
		DxWindow* dxWindow_ = DxWindow::GetInstance();
		//DirectXクラス生成
		Dx12* dx_ = Dx12::GetInstance();
		//inputクラス生成
		Input* input_ = Input::GetInstance();

		ShaderResource* descriptor_ = ShaderResource::GetInstance();

		Depth* depth_;
		//ルートパラメータ
		RootParameter* rootparamsPtr_;

		PipeLineManager* pipeline_;

		//PipelineRootSignature* obj3dPipelinePtr;

		//PipelineRootSignature* spritePipelinePtr;

		//PipelineRootSignature* particlePipelinePtr;

		LightGroup* lights_ = LightGroup::GetInstance();

		//描画前処理と描画後処理
		MCB::Draw* draw_ = Draw::GetInstance();

		//FBXLoader* fbxLoader = FBXLoader::GetInstance();
	
		//サウンドマネージャー
		SoundManager* soundManager_ = SoundManager::GetInstance();
		ModelManager* modelManager_ = ModelManager::GetInstance();
		//クリアカラー
#pragma endregion DirectX基礎機能群
		bool sceneEnd_ = false;
		Byte7 pad;
		std::shared_ptr<IScene> nextScene_ = nullptr;

		ICamera* viewCamera_;

		TextureManager* loader_ = TextureManager::GetInstance();

		std::unique_ptr<PostEffect> postEffect_ = std::make_unique<PostEffect>();
	public:
		ImguiManager imgui_;
		Byte3 pad2;
		float clearColor_[4] = { 0.0f,0.25f, 0.5f,0.0f }; // 青っぽい色
		Byte4 pad3;
		virtual ~IScene();
		IScene();
		IScene(const IScene&) = delete;
		IScene& operator=(const IScene&) = delete;

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
		void ImGuiDraw();
		virtual void ImGuiUpdate() = 0;
		virtual std::unique_ptr<IScene> GetNextScene() = 0;
		virtual void CheckAllColision() = 0;
		bool GetIsSceneEnd();

		Depth* GetDepthPtr();
		Depth& GetDepth();
		//ルートパラメータ
		RootParameter* GetRootparamsPtr();
		
		PipeLineManager* GetpipelinePtr();
		PipeLineManager& Getpipeline();
	};

}

